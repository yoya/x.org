/*
 * $XConsortium: Xdrawing.cxx,v 1.5 94/09/01 18:45:46 matt Exp $
 */

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

/*
 * X-dependent drawing operations
 */

#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/Impls/charstr.h>
#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/Impls/Xdisplay.h>
#include <X11/Fresco/Impls/Xdrawing.h>
#include <X11/Fresco/Impls/Xpainter.h>
#include <X11/Fresco/Impls/Xraster.h>
#include <X11/Fresco/Impls/Xwindow.h>
#include <X11/Fresco/Impls/Xlib.h>
#include <X11/Fresco/OS/math.h>
#include <X11/Fresco/OS/memory.h>
#include <X11/Fresco/OS/table.h>
#include <X11/Xatom.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Sony NEWS-OS 6.0 has conflicting prototypes for abs() in math.h
 * and stdlib.h, so you cannot include both headers in any C++ source
 * file.  Until that bug is fixed (in NEWS-OS 6.0.1), we'll have to
 * explicitly define the symbols we need from math.h on the Sony.
 * On other platforms we can just include math.h
 */

#if defined(sony) && defined(SVR4)
extern "C" double sqrt(double);
#else
#include <math.h>
#endif

/*
 * X logical font description manipulator
 */

static const int max_spec = 1000;
typedef unsigned int FontSpecIndex;

class FontSpec {
public:
    FontSpec(const char*);
    FontSpec(const FontSpec&);
    FontSpec();
    ~FontSpec();

    void operator =(const FontSpec&);

    enum Field {
	foundry, family, weight, slant, setwidth, astyle, pixels, points,
	xdpi, ydpi, spacing, width, set, last, max_index
    };

    const char* full_spec() const;
    Boolean is_scalable();

    char* field(FontSpecIndex) const;
    void field(FontSpecIndex, char*);

    static Boolean valid(const char*);
protected:
    char specs_[max_spec];
    char* fields_[max_index];
    Boolean updated_;

    void copy(const FontSpec&);
    void copy(const char*);
    void update();
};

/*
 * Two useful array macros: determine the number of elements and
 * return the address of the end of the array (after last element).
 * We assume the compiler will fold any constant arithmetic.
 */

#define array_nelements(array) (sizeof(array) / sizeof(array[0]))
#define array_end(array) (&array[array_nelements(array)])

/*
 * This function tests whether a matrix is only a translation.
 * Something like this probably should be provided directly
 * by the Transform interface.
 */
static Boolean is_translation(TransformRef t) {
    Transform::Matrix m;
    t->store_matrix(m);
    return m[0][0] == 1 && m[0][1] == 0 && m[1][0] == 0 && m[1][1] == 1;
}

/*
 * Returns true if the two transforms differ at most by a translation.
 */
static Boolean translated_only(Transform_in t1, Transform_in t2) {
    Transform::Matrix m1, m2;
    t1->store_matrix(m1);
    t2->store_matrix(m2);
    return (
	m1[0][0] == m2[0][0] && m1[0][1] == m2[0][1] &&
	m1[1][0] == m2[1][0] && m1[1][1] == m2[1][1]
    );
}

/*
 * Translate t2 by t1.
 */
static void translate_remainder(Transform_in t1, Transform_in t2) {
    Transform::Matrix m1, m2;
    t1->store_matrix(m1);
    t2->store_matrix(m2);
    m2[2][0] = m1[2][0];
    m2[2][1] = m1[2][1];
    t2->load_matrix(m2);
}

BrushImpl::BrushImpl(Coord w) { init(nil, 0, w); }
BrushImpl::BrushImpl(const Long* p, Long c, Coord w) { init(p, c, w); }

BrushImpl::BrushImpl(Long pat, Coord w) {
    Long dash[16];
    Long count;

    calc_dashes(pat, dash, count);
    init(dash, count, w);
}

BrushImpl::~BrushImpl() {
    delete dash_list_;
}

//+ BrushImpl(FrescoObject::=object_.)
Long BrushImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag BrushImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void BrushImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void BrushImpl::disconnect() {
    object_.disconnect();
}
void BrushImpl::notify_observers() {
    object_.notify_observers();
}
void BrushImpl::update() {
    object_.update();
}
//+

//+ BrushImpl(Brush::width)
Coord BrushImpl::width() {
    return width_;
}

//+ BrushImpl(Brush::hash)
ULong BrushImpl::hash() {
    return 0;
}

//+ BrushImpl(Brush::equal)
Boolean BrushImpl::equal(Brush_in b) {
    if (is_nil(b) || hash() != b->hash()) {
	return false;
    }
    /* should compare values */
    return this == b;
}

void BrushImpl::info(Coord& w, char*& dash_list, Long& count) {
    w = width_;
    dash_list = dash_list_;
    count = dash_count_;
}

void BrushImpl::init(const Long* pattern, Long count, Coord w) {
    width_ = w;
    dash_count_ = count;
    if (count > 0) {
        dash_list_ = new char[count];
        for (Long i = 0; i < count; i++) {
            dash_list_[i] = char(pattern[i]);
        }
    } else {
        dash_list_ = nil;
    }
}

void BrushImpl::calc_dashes(Long pat, Long* dash, Long& count) {
    ULong p = pat & 0xffff;

    if (p == 0 || p == 0xffff) {
        count = 0;
    } else {
        const ULong MSB = 1 << 15;
        while ((p & MSB) == 0) {
            p <<= 1;
        }

        if (p == 0x5555 || p == 0xaaaa) {
            dash[0] = 1;
            dash[1] = 3;
            count = 2;
        } else if (p == 0xaaaa) {
            dash[0] = 1;
            dash[1] = 1;
            count = 2;
        } else if (p == 0xcccc) {
            dash[0] = 2;
            dash[1] = 2;
            count = 2;
        } else if (p == 0xeeee) {
            dash[0] = 3;
            dash[1] = 1;
            count = 2;
        } else {
           ULong m = MSB;
           Long index = 0;
           while (m != 0) {
                /* count the consecutive one bits */
                int length = 0;
                while (m != 0 && (p & m) != 0) {
                    ++length;
                    m >>= 1;
                }
                dash[index++] = length;

                /* count the consecutive zero bits */
                length = 0;
                while (m != 0 && (p & m) == 0) {
                    ++length;
                    m >>= 1;
                }
                if (length > 0) {
                    dash[index++] = length;
                }
            }
            count = index;
        }
    }
}

ColorImpl::ColorImpl(
    Color::Intensity r, Color::Intensity g, Color::Intensity b
) {
    red_ = r;
    green_ = g;
    blue_ = b;
}

ColorImpl::~ColorImpl() { }

//+ ColorImpl(FrescoObject::=object_.)
Long ColorImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag ColorImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void ColorImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void ColorImpl::disconnect() {
    object_.disconnect();
}
void ColorImpl::notify_observers() {
    object_.notify_observers();
}
void ColorImpl::update() {
    object_.update();
}
//+

//+ ColorImpl(Color::rgb)
void ColorImpl::rgb(Intensity& r, Intensity& g, Intensity& b) {
    r = red_;
    g = green_;
    b = blue_;
}

/*
 * Hash function generates 18 distinct bits, 6 each from rgb.
 * The idea is to "promote" collisions a little bit, so that colors
 * that are indistinguishable will match.
 */

//+ ColorImpl(Color::hash)
ULong ColorImpl::hash() {
    Long r = Long(64 * red_) & 0x3f;
    Long g = Long(64 * green_) & 0x3f;
    Long b = Long(64 * blue_) & 0x3f;
    return (r << 12) | (g << 6) | b;
}

//+ ColorImpl(Color::equal)
Boolean ColorImpl::equal(Color_in c) {
    if (is_nil(c)) {
	return false;
    }
    Color::Intensity r, g, b;
    c->rgb(r, g, b);
    float tol = 1e-2;
    /* convoluted testing to workaround compiler inline problem */
    Boolean result = Math::equal(red_, r, tol);
    if (result) {
	result = Math::equal(green_, g, tol);
	if (result) {
	    result = Math::equal(blue_, b, tol);
	}
    }
    return result;
}

static inline ULong num_chars(XFontStruct* fs) {
    return (
	(fs->max_byte1 - fs->min_byte1 + 1) *
	(fs->max_char_or_byte2 - fs->min_char_or_byte2 + 1)
    );
}

static ULong char_index(CharCode c, XFontStruct* fs) {
    if (fs->min_byte1 == fs->max_byte1) {
	return ULong(c - fs->min_char_or_byte2);
    }
    ULong width = fs->max_char_or_byte2 - fs->min_char_or_byte2 + 1;
    return (
	((c / width) - fs->min_byte1) * width +
	(c % width) - fs->min_char_or_byte2
    );
}

FontImpl::FontImpl(DisplayImpl* d, const char* name) {
    name_ = new CharStringImpl(name);
    encoding_ = nil;
    point_size_ = 0;
    scale_ = 1.0;
    xdisplay_ = d->xdisplay();
    xfont_ = nil;
    rasters_ = nil;
}

FontImpl::~FontImpl() {
    Fresco::unref(name_);
    Fresco::unref(encoding_);
    if (rasters_ != nil) {
	for (Long i = num_chars(xfont_) - 1; i >= 0; i--) {
	    Fresco::unref(rasters_[i]);
	}
	delete [] rasters_;
    }
    if (xfont_ != nil) {
	XFreeFont(xdisplay_, xfont_);
    }
}

//+ FontImpl(FrescoObject::=object_.)
Long FontImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag FontImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void FontImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void FontImpl::disconnect() {
    object_.disconnect();
}
void FontImpl::notify_observers() {
    object_.notify_observers();
}
void FontImpl::update() {
    object_.update();
}
//+

//+ FontImpl(Font::hash)
ULong FontImpl::hash() {
    return name_->hash();
}

//+ FontImpl(Font::equal)
Boolean FontImpl::equal(Font_in f) {
    return f == this || name_->equal(_tmp(f->name()));
}

//+ FontImpl(Font::name)
CharString_return FontImpl::name() {
    return CharString::_duplicate(name_);
}

//+ FontImpl(Font::encoding)
CharString_return FontImpl::encoding() {
    return CharString::_duplicate(encoding_);
}

//+ FontImpl(Font::point_size)
Coord FontImpl::point_size() {
    load();
    return point_size_;
}

//+ FontImpl(Font::font_info)
void FontImpl::font_info(Font::Info& i) {
    load();
    XFontStruct* xf = xfont_;
    Coord s = scale_;
    i.left_bearing = to_coord(xf->max_bounds.lbearing);
    i.right_bearing = to_coord(xf->max_bounds.rbearing);
    i.width = to_coord(xf->max_bounds.width);
    i.ascent = to_coord(xf->ascent);
    i.descent = to_coord(xf->descent);
    i.font_ascent = i.ascent;
    i.font_descent = i.descent;
    i.height = i.font_ascent + i.font_descent;
}

//+ FontImpl(Font::char_info)
void FontImpl::char_info(CharCode c, Font::Info& i) {
    load();
    Coord s = scale_;
    XCharStruct xc;
    XChar2b xc2b;
    xc2b.byte1 = (unsigned char)((c & 0xff00) >> 8);
    xc2b.byte2 = (unsigned char)(c & 0xff);
    int dir, asc, des;
    XTextExtents16(xfont_, &xc2b, 1, &dir, &asc, &des, &xc);
    i.left_bearing = to_coord(-xc.lbearing);
    i.right_bearing = to_coord(xc.rbearing);
    i.ascent = to_coord(xc.ascent);
    i.descent = to_coord(xc.descent);
    i.font_ascent = to_coord(xfont_->ascent);
    i.font_descent = to_coord(xfont_->descent);
    i.width = to_coord(xc.width);
    i.height = i.ascent + i.descent;
}

//+ FontImpl(Font::string_info)
void FontImpl::string_info(CharString_in s, Font::Info& i) {
    load();
    CharStringBuffer buf(s);
    const char* cp = buf.string();
    Long n = buf.length();
    XCharStruct xc;
    int dir, asc, des;
    XTextExtents(xfont_, cp, int(n), &dir, &asc, &des, &xc);
    i.left_bearing = to_coord(-xc.lbearing);
    i.right_bearing = to_coord(xc.rbearing);
    i.ascent = to_coord(xc.ascent);
    i.descent = to_coord(xc.descent);
    i.font_ascent = to_coord(xfont_->ascent);
    i.font_descent = to_coord(xfont_->descent);
    i.width = to_coord(xc.width);
    i.height = i.ascent + i.descent;
}

void FontImpl::load() {
    if (xfont_ != nil) {
	return;
    }
    CharStringBuffer buf(name_);
    xfont_ = XLoadQueryFont(xdisplay_, buf.string());
    if (xfont_ == nil) {
	/* raise exception? */
	return;
    }
    unsigned long value;
    if (XGetFontProperty(xfont_, XA_POINT_SIZE, &value)) {
	point_size_ = Coord(value) / 10.0;
    }
    if (XGetFontProperty(xfont_, XA_RESOLUTION, &value)) {
	scale_ = (100.0 * 72.0 / 72.27) / Coord(value);
    } else {
	FontSpec spec(buf.string());
	scale_ = 72.0 / atol(spec.field(FontSpec::ydpi));
    }
}

RasterRef FontImpl::bitmap(CharCode c) {
    load();
    ULong index = char_index(c, xfont_);
    if (rasters_ == nil) {
	ULong size = num_chars(xfont_);
	rasters_ = new RasterRef[size];
	Memory::zero(rasters_, size * sizeof(RasterRef));
    } else {
	RasterRef r = rasters_[index];
	if (is_not_nil(r)) {
	    Fresco::ref(r);
	    return r;
	}
    }
    Coord s = scale_;
    XCharStruct xc;
    XChar2b xc2b;
    xc2b.byte1 = (unsigned char)((c & 0xff00) >> 8);
    xc2b.byte2 = (unsigned char)(c & 0xff);
    int dir, asc, des;
    XTextExtents16(xfont_, &xc2b, 1, &dir, &asc, &des, &xc);

    int cwidth = xc.rbearing - xc.lbearing;
    int cheight = xc.ascent + xc.descent;
    Pixmap pixmap = XCreatePixmap(
	xdisplay_, DefaultRootWindow(xdisplay_), cwidth, cheight, 1
    );
    XGCValues values;
    values.font = xfont_->fid;
    values.background = 0;
    values.foreground = 0;
    GC xgc = XCreateGC(
	xdisplay_, pixmap, GCForeground | GCBackground | GCFont, &values
    );
    XFillRectangle(xdisplay_, pixmap, xgc, 0, 0, cwidth, cheight);
    XSetForeground(xdisplay_, xgc, 1);
    XDrawString16(xdisplay_, pixmap, xgc, -xc.lbearing, xc.ascent, &xc2b, 1);
    XFreeGC(xdisplay_, xgc);
    DrawingKit::Data8 data;
    data._buffer = nil;
    RasterBitmap* rb = new RasterBitmap(
	data, cheight, cwidth, xc.descent, -xc.lbearing, scale_
    );
    rb->read_drawable(xdisplay_, pixmap, 0, 0);
    XFreePixmap(xdisplay_, pixmap);
    rasters_[index] = rb;
    Fresco::ref(rb);
    return rb;
}

Coord FontImpl::to_coord(PixelCoord c) {
    return scale_ * Coord(c);
}

Coord FontImpl::average_width() {
    load();
    XFontStruct* f = xfont_;
    Coord width = 0.0;
    if (f->per_char != nil) {
	PixelCoord pwidth = 0, n = 0;
	Long max = f->max_char_or_byte2 - f->min_char_or_byte2;
	for (Long i = 0; i < max; i++) {
	    PixelCoord w = f->per_char[i].width;
	    if (w > 0) {
		pwidth += w;
		++n;
	    }
	}
	if (n > 0) {
	    width = to_coord(pwidth / n);
	}
    }
    return width;
}

PatternImpl::PatternImpl(ULong p) {
    stipple_._maximum = 4;
    stipple_._length = 4;
    stipple_._buffer = new Long[4];
    stipple_._buffer[0] = (p & 0xf000) >> 12;
    stipple_._buffer[1] = (p & 0x0f00) >> 8;
    stipple_._buffer[2] = (p & 0x00f0) >> 4;
    stipple_._buffer[3] = (p & 0x000f);
}

PatternImpl::PatternImpl(const DrawingKit::Data32& d) {
    stipple_._maximum = d._length;
    stipple_._length = d._length;
    stipple_._buffer = new Long[d._length];
    Memory::copy(d._buffer, stipple_._buffer, sizeof(Long) * d._length);
}

PatternImpl::~PatternImpl() {
    /* avoid implicit deallocation */
    stipple_._buffer = nil;
}

//+ PatternImpl(FrescoObject::=object_.)
Long PatternImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag PatternImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void PatternImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void PatternImpl::disconnect() {
    object_.disconnect();
}
void PatternImpl::notify_observers() {
    object_.notify_observers();
}
void PatternImpl::update() {
    object_.update();
}
//+

//+ PatternImpl(Pattern::equal)
Boolean PatternImpl::equal(Pattern_in p) {
    Boolean b = false;
    if (p == this) {
	b = true;
    } else {
	PatternImpl* i = PatternImpl::_narrow(p);
	if (i != nil) {
	    Long n = stipple_._length;
	    DrawingKit::Data32& d = i->stipple_;
	    b = (
		n == d._length &&
		Memory::compare(
		    stipple_._buffer, d._buffer, n * sizeof(Long)
		) == 0
	    );
	}
	/* should do something reasonable here, but what? */
    }
    return b;
}

//+ PatternImpl(Pattern::hash)
ULong PatternImpl::hash() {
    return 0;
}

//+ PatternImpl(Pattern::stipple)
void PatternImpl::stipple(DrawingKit::Data32& d) {
    d = stipple_;
}

/*
 * We only need this silly MatrixStackElement because the compiler
 * (language) can't deal with functions that return arrays.
 */

struct MatrixStackElement {
    Transform::Matrix matrix;
};

declareList(MatrixStack,MatrixStackElement)
implementList(MatrixStack,MatrixStackElement)

declarePtrList(ClippingStack,RegionImpl)
implementPtrList(ClippingStack,RegionImpl)

inline Long DefaultPainterImpl::clip_index(Long i) {
    return i % (sizeof(free_clippers_) / sizeof(free_clippers_[0]));
}

DefaultPainterImpl::DefaultPainterImpl() {
    brush_ = nil;
    color_ = nil;
    font_ = nil;
    pattern_ = nil;
    matrix_ = new TransformImpl;
    matrix_tag_ = matrix_->attach(this);
    transforms_ = new MatrixStack;
    clipping_ = nil;
    clippers_ = new ClippingStack;
    free_clipper_head_ = 0;
    free_clipper_tail_ = 0;
}

DefaultPainterImpl::~DefaultPainterImpl() {
    Fresco::unref(brush_);
    Fresco::unref(color_);
    Fresco::unref(font_);
    Fresco::unref(pattern_);
    matrix_->detach(matrix_tag_);
    Fresco::unref(matrix_);
    delete transforms_;
    Fresco::unref(clipping_);
    for (ListItr(ClippingStack) j(*clippers_); j.more(); j.next()) {
	Fresco::unref(j.cur());
    }
    delete clippers_;
    for (
	Long i = free_clipper_head_;
	i != free_clipper_tail_;
	i = clip_index(i + 1)
    ) {
	Fresco::unref(free_clippers_[i]);
    }
}

//+ DefaultPainterImpl(FrescoObject::=object_.)
Long DefaultPainterImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag DefaultPainterImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void DefaultPainterImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void DefaultPainterImpl::disconnect() {
    object_.disconnect();
}
void DefaultPainterImpl::notify_observers() {
    object_.notify_observers();
}
void DefaultPainterImpl::update() {
    object_.update();
}
//+

//+ DefaultPainterImpl(Painter::to_coord)
Coord DefaultPainterImpl::to_coord(PixelCoord p) { return Coord(p); }

//+ DefaultPainterImpl(Painter::to_pixels)
PixelCoord DefaultPainterImpl::to_pixels(Coord c) { return PixelCoord(c); }

//+ DefaultPainterImpl(Painter::to_pixels_coord)
Coord DefaultPainterImpl::to_pixels_coord(Coord c) { return c; }

//+ DefaultPainterImpl(Painter::begin_path)
void DefaultPainterImpl::begin_path() { }

//+ DefaultPainterImpl(Painter::move_to)
void DefaultPainterImpl::move_to(Coord, Coord) { }

//+ DefaultPainterImpl(Painter::line_to)
void DefaultPainterImpl::line_to(Coord, Coord) { }

//+ DefaultPainterImpl(Painter::curve_to)
void DefaultPainterImpl::curve_to(Coord, Coord, Coord, Coord, Coord, Coord) { }

//+ DefaultPainterImpl(Painter::close_path)
void DefaultPainterImpl::close_path() { }

//+ DefaultPainterImpl(Painter::current_brush=b)
void DefaultPainterImpl::current_brush(Brush_in b) {
    if (brush_ != b) {
	Fresco::unref(brush_);
	brush_ = Brush::_duplicate(b);
	set_brush();
    }
}

//+ DefaultPainterImpl(Painter::current_brush?)
BrushRef DefaultPainterImpl::current_brush() {
    return Brush::_duplicate(brush_);
}

//+ DefaultPainterImpl(Painter::current_color=c)
void DefaultPainterImpl::current_color(Color_in c) {
    if (color_ != c) {
	Fresco::unref(color_);
	color_ = Color::_duplicate(c);
	set_color();
    }
}

//+ DefaultPainterImpl(Painter::current_color?)
ColorRef DefaultPainterImpl::current_color() {
    return Color::_duplicate(color_);
}

//+ DefaultPainterImpl(Painter::current_font=f)
void DefaultPainterImpl::current_font(Font_in f) {
    if (font_ != f) {
	Fresco::unref(font_);
	font_ = Font::_duplicate(f);
	set_font();
    }
}

//+ DefaultPainterImpl(Painter::current_font?)
FontRef DefaultPainterImpl::current_font() {
    return Font::_duplicate(font_);
}

//+ DefaultPainterImpl(Painter::current_pattern=p)
void DefaultPainterImpl::current_pattern(Pattern_in p) {
    if (pattern_ != p) {
	Fresco::unref(pattern_);
	pattern_ = Pattern::_duplicate(p);
	set_pattern();
    }
}

//+ DefaultPainterImpl(Painter::current_pattern?)
PatternRef DefaultPainterImpl::current_pattern() {
    return Pattern::_duplicate(pattern_);
}

//+ DefaultPainterImpl(Painter::stroke)
void DefaultPainterImpl::stroke() { }

//+ DefaultPainterImpl(Painter::fill)
void DefaultPainterImpl::fill() { }

//+ DefaultPainterImpl(Painter::line)
void DefaultPainterImpl::line(Coord, Coord, Coord, Coord) { }

//+ DefaultPainterImpl(Painter::rect)
void DefaultPainterImpl::rect(Coord, Coord, Coord, Coord) { }

//+ DefaultPainterImpl(Painter::fill_rect)
void DefaultPainterImpl::fill_rect(Coord, Coord, Coord, Coord) { }

//+ DefaultPainterImpl(Painter::character)
void DefaultPainterImpl::character(CharCode, Coord, Coord, Coord) { }

//+ DefaultPainterImpl(Painter::image)
void DefaultPainterImpl::image(Raster_in, Coord, Coord) { }

//+ DefaultPainterImpl(Painter::stencil)
void DefaultPainterImpl::stencil(Raster_in, Coord, Coord) { }

//+ DefaultPainterImpl(Painter::current_matrix=t)
void DefaultPainterImpl::current_matrix(Transform_in t) {
    flush_text();
    matrix_->load(t);
}

//+ DefaultPainterImpl(Painter::current_matrix?)
TransformRef DefaultPainterImpl::current_matrix() {
    return Transform::_duplicate(matrix_);
}

//+ DefaultPainterImpl(Painter::push_matrix)
void DefaultPainterImpl::push_matrix() {
    transforms_->prepend(*((MatrixStackElement*)(matrix_->matrix())));
}

//+ DefaultPainterImpl(Painter::pop_matrix)
void DefaultPainterImpl::pop_matrix() {
    MatrixStack* s = transforms_;
    if (s->count() == 0) {
	/*
	 * Underflow -- should raise an exception.
	 */
	return;
    }
    flush_text();
    matrix_->load_matrix(s->item_ref(0).matrix);
    s->remove(0);
}

//+ DefaultPainterImpl(Painter::premultiply)
void DefaultPainterImpl::premultiply(Transform_in t) {
    flush_text();
    matrix_->premultiply(t);
}

//+ DefaultPainterImpl(Painter::clip)
void DefaultPainterImpl::clip() {
    /* not implemented */
}

//+ DefaultPainterImpl(Painter::clip_rect)
void DefaultPainterImpl::clip_rect(Coord x0, Coord y0, Coord x1, Coord y1) {
    RegionImpl region;
    RegionImpl* r = (clipping_ == nil) ? new_clip() : &region;
    r->defined_ = true;
    r->lower_.x = x0; r->lower_.y = y0; r->lower_.z = 0.0;
    r->upper_.x = x1; r->upper_.y = y1; r->upper_.z = 0.0;
    if (!matrix_->identity()) {
	r->apply_transform(matrix_);
    }
    if (clipping_ == nil) {
	clipping_ = r;
    } else {
	clipping_->merge_intersect(r);
    }
    set_clip();
}

//+ DefaultPainterImpl(Painter::push_clipping)
void DefaultPainterImpl::push_clipping() {
    clippers_->prepend(clipping_);
    if (is_not_nil(clipping_)) {
	RegionImpl* r = new_clip();
	r->defined_ = true;
	r->lower_ = clipping_->lower_;
	r->upper_ = clipping_->upper_;
	clipping_ = r;
    }
}

RegionImpl* DefaultPainterImpl::new_clip() {
    RegionImpl* r;
    if (free_clipper_head_ != free_clipper_tail_) {
	r = free_clippers_[free_clipper_head_];
	free_clipper_head_ = clip_index(free_clipper_head_ + 1);
    } else {
	r = new RegionImpl;
    }
    return r;
}

//+ DefaultPainterImpl(Painter::pop_clipping)
void DefaultPainterImpl::pop_clipping() {
    ClippingStack* s = clippers_;
    if (s->count() == 0) {
	/*
	 * Underflow -- should raise an exception.
	 */
	return;
    }
    flush_text();
    free_clip(clipping_);
    clipping_ = s->item(0);
    s->remove(0);
    reset_clip();
}

void DefaultPainterImpl::free_clip(RegionImpl* r) {
    Long new_tail = clip_index(free_clipper_tail_ + 1);
    if (new_tail != free_clipper_head_) {
	free_clippers_[free_clipper_tail_] = r;
	free_clipper_tail_ = new_tail;
    } else {
	Fresco::unref(r);
    }
}

/*
 * Determine if the given region is (at least partially) visible
 * given the current clipping region.
 */

//+ DefaultPainterImpl(Painter::is_visible)
Boolean DefaultPainterImpl::is_visible(Region_in r) {
    Boolean b = is_nil(clipping_);
    if (!b) {
	RegionRef rr;
	RegionImpl bounds;
	if (matrix_->identity()) {
	    rr = r;
	} else {
	    rr = &bounds;
	    rr->copy(r);
	    rr->apply_transform(matrix_);
	}
	b = clipping_->intersects(rr);
    }
    return b;
}

//+ DefaultPainterImpl(Painter::visible)
Region_return DefaultPainterImpl::visible() {
    return Region::_duplicate(clipping_);
}

/*
 * The comment and page_number operations are typically
 * only relevent for a printer.
 */

//+ DefaultPainterImpl(Painter::comment)
void DefaultPainterImpl::comment(CharString_in) { }

//+ DefaultPainterImpl(Painter::page_number)
void DefaultPainterImpl::page_number(CharString_in) { }

void DefaultPainterImpl::set_clip() { }
void DefaultPainterImpl::reset_clip() { }
void DefaultPainterImpl::flush_text() { }

void DefaultPainterImpl::set_brush() { }
void DefaultPainterImpl::set_color() { }
void DefaultPainterImpl::set_font() { }
void DefaultPainterImpl::set_pattern() { }

/* class XPainterImpl */

XPainterImpl::XPainterImpl(WindowImpl* w, ScreenImpl* s) {
    window_ = w;
    screen_ = s;
    DisplayImpl* display = w->display();
    xdisplay_ = display->xdisplay();
    double_buffered_ = false;
    xdrawable_ = nil;
    xfrontbuffer_ = nil;
    xbackbuffer_ = nil;
    xgc_ = nil;
    xfrontgc_ = nil;
    pixels_ = s->to_coord(1);
    points_ = 1 / pixels_;
    smoothness_ = 10.0;
    matrix_modified_ = false;
    point_ = new XPoint[25];
    cur_point_ = point_;
    end_point_ = point_ + 25;
    subpath_ = new SubPathInfo[5];
    cur_subpath_ = subpath_;
    end_subpath_ = subpath_ + 5;
    init_fonts();
    init_items();
    tx0_ = 0;
    ty0_ = 0;
    tx_ = 0.0;
    ty_ = 0.0;
    tsx_ = 0.0;
    Fresco* f = display->fresco();
    font_tol_ = 0.3;
    StyleValue_var a = _tmp(f->fresco_style())->resolve(
	Fresco::tmp_string_ref("fontTolerance")
    );
    if (is_not_nil(a)) {
	double d;
	a->read_real(d);
	font_tol_ = float(d);
    }
    pwidth_ = 0;
    pheight_ = 0;
}

/*
 * The assumption here is the we were constructred without being given
 * a refcount for the WindowImpl and ScreenImpl, so we don't need
 * to unref them.  We also must re-fetch the xdisplay_ data member,
 * as the display may already have been closed.
 */

XPainterImpl::~XPainterImpl() {
    xdisplay_ = window_->display()->xdisplay();
    cleanup();
    close_fonts();
    delete point_;
    delete subpath_;
}

//+ XPainterImpl(FrescoObject::update)
void XPainterImpl::update() {
    matrix_modified_ = true;
    /* avoid redundant update calls */
    matrix_->detach(matrix_tag_);
    matrix_tag_ = 0;
}

//+ XPainterImpl(Painter::to_coord)
Coord XPainterImpl::to_coord(PixelCoord p) {
    return inline_to_coord(p);
}

//+ XPainterImpl(Painter::to_pixels)
PixelCoord XPainterImpl::to_pixels(Coord c) {
    return inline_to_pixels(c);
}

//+ XPainterImpl(Painter::to_pixels_coord)
Coord XPainterImpl::to_pixels_coord(Coord c) {
    return inline_to_pixels_coord(c);
}

Boolean XPainterImpl::straight(
    Coord x0, Coord y0, Coord x1, Coord y1,
    Coord x2, Coord y2, Coord x3, Coord y3
) {
    float f = (
        (x1 + x2) * (y0 - y3) + (y1 + y2) * (x3 - x0) +
        2 * (x0 * y3 - y0 * x3)
    );
    return (f * f) < smoothness_;
}

inline Coord XPainterImpl::mid(Coord a, Coord b) {
    return (a + b) * 0.5;
}

//+ XPainterImpl(Painter::begin_path)
void XPainterImpl::begin_path() {
    path_cur_x_ = 0;
    path_cur_y_ = 0;
    XPoint* xp = point_;
    xp->x = 0;
    xp->y = 0;
    cur_point_ = xp;
    SubPathInfo* sp = subpath_;
    cur_subpath_ = sp;
    cur_subpath_->start = xp - point_;
    cur_subpath_->end = cur_subpath_->start;
    cur_subpath_->closed = false;
}

//+ XPainterImpl(Painter::move_to)
void XPainterImpl::move_to(Coord x, Coord y) {
    path_cur_x_ = x;
    path_cur_y_ = y;
    Vertex v;
    v.x = x; v.y = y; v.z = 0;
    if (!matrix_->identity()) {
	matrix_->transform_vertex(v);
    }

    XPoint* xp;
    /* Test for two move_to on the row */
    if (cur_point_ - point_ == cur_subpath_->start + 1) {
	cur_point_ = point_ + cur_subpath_->start;
	xp = cur_point_;
    } else if (cur_point_ != point_) {
	/* start a new subpath */
	if (!cur_subpath_->closed) {
	    /* do some work to close the path */
	    cur_subpath_->end = cur_point_ - point_;
	    close_path();
	    cur_subpath_->closed = false;
	}
	next_subpath();
	xp = next_point();
	cur_subpath_->start = xp - point_;
	cur_subpath_->end = 0; /* means not terminated yet */
	cur_subpath_->closed = false;
    } else {
	xp = next_point();
    }

    xp->x = short(inline_to_pixels(v.x));
    xp->y = short(pheight_ - inline_to_pixels(v.y));
    cur_point_ = xp + 1;
}

static void constrain_point(XPoint* xp, PixelCoord x, PixelCoord y) {
    if (x < SHRT_MIN) {
	xp->x = SHRT_MIN;
    } else if (x > SHRT_MAX) {
	xp->x = SHRT_MAX;
    } else {
	xp->x = XCoord(x);
    }
    if (y < SHRT_MIN) {
	xp->y = SHRT_MIN;
    } else if (y > SHRT_MAX) {
	xp->y = SHRT_MAX;
    } else {
	xp->y = XCoord(y);
    }
}

//+ XPainterImpl(Painter::line_to)
void XPainterImpl::line_to(Coord x, Coord y) {
    if (cur_subpath_->closed) {
	return;
    }

    path_cur_x_ = x;
    path_cur_y_ = y;
    Vertex v;
    v.x = x; v.y = y; v.z = 0;
    if (!matrix_->identity()) {
	matrix_->transform_vertex(v);
    }
    PixelCoord px = inline_to_pixels(v.x);
    PixelCoord py = pheight_ - inline_to_pixels(v.y);
    constrain_point(next_point(), px, py);
}

//+ XPainterImpl(Painter::curve_to)
void XPainterImpl::curve_to(Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2) {
    Vertex v, v1, v2, pv;
    v.x = x;
    v.y = y;
    v1.x = x1;
    v1.y = y1;
    v2.x = x2;
    v2.y = y2;
    pv.x = path_cur_x_;
    pv.y = path_cur_y_;
    if (!matrix_->identity()) {
	matrix_->transform_vertex(v);
	matrix_->transform_vertex(v1);
	matrix_->transform_vertex(v2);
	matrix_->transform_vertex(pv);
    }
    curve_pt(pv.x, pv.y, v.x, v.y, v1.x, v1.y, v2.x, v2.y);
    path_cur_x_ = x;
    path_cur_y_ = y;
}

//+ XPainterImpl(Painter::close_path)
void XPainterImpl::close_path() {
    if (cur_subpath_->closed) {
	return;
    }

    XPoint* start = point_ + cur_subpath_->start;
    XPoint* xp = next_point();
    *xp = *start;

    cur_subpath_->end = cur_point_ - point_;
    if (cur_subpath_ != subpath_) {
	/* return to the first point of the first subpath */
	xp = next_point();
	*xp = *point_;
    }
    cur_subpath_->closed = true;
}

//+ XPainterImpl(Painter::line)
void XPainterImpl::line(Coord x0, Coord y0, Coord x1, Coord y1) {
    begin_path();
    move_to(x0, y0);
    line_to(x1, y1);
    stroke();
}

//+ XPainterImpl(Painter::rect)
void XPainterImpl::rect(Coord x0, Coord y0, Coord x1, Coord y1) {
    begin_path();
    move_to(x0, y0);
    line_to(x0, y1);
    line_to(x1, y1);
    line_to(x1, y0);
    close_path();
    stroke();
}

//+ XPainterImpl(Painter::fill_rect)
void XPainterImpl::fill_rect(Coord x0, Coord y0, Coord x1, Coord y1) {
    begin_path();
    move_to(x0, y0);
    line_to(x0, y1);
    line_to(x1, y1);
    line_to(x1, y0);
    close_path();
    fill();
}

void XPainterImpl::set_brush() {
    /*
     * Should do (checked) narrow instead of cast.
     */
    BrushImpl* br = (BrushImpl*)brush_;
    Coord w;
    char* dashes;
    Long ndashes;
    br->info(w, dashes, ndashes);
    short p = short(inline_to_pixels(w));
    XDisplay* dpy = xdisplay_;
    GC gc = xgc_;
    if (dashes == nil) {
	XSetLineAttributes(dpy, gc, p, LineSolid, CapButt, JoinMiter);
    } else {
	XSetLineAttributes(dpy, gc, p, LineOnOffDash, CapButt, JoinMiter);
	XSetDashes(dpy, gc, 0, dashes, int(ndashes));
    }
}

void XPainterImpl::set_color() {
    flush_text();
    // need a table here from Color -> pixel
    XColor xc;
    Color::Intensity r, g, b;
    color_->rgb(r, g, b);
    xc.red = ColorImpl::to_short(r);
    xc.green = ColorImpl::to_short(g);
    xc.blue = ColorImpl::to_short(b);
    screen_->find_color(*window_->visual(), xc);
    XSetForeground(xdisplay_, xgc_, xc.pixel);
}

void XPainterImpl::set_font() {
    XPainterImpl::FontInfo* info = open_font(font_);
    if (info->charsize != fontinfo_->charsize) {
	flush_text();
    }
    info->age = 0;
    add_item();
    item_->font = info->xfont;
    fontinfo_ = info;
}

void XPainterImpl::set_pattern() {
}

//+ XPainterImpl(Painter::fill)
void XPainterImpl::fill() {
    int n = int(cur_point_ - point_);
    if (n <= 2) {
	return;
    }
    XDisplay* dpy = xdisplay_;
    XDrawable d = xdrawable_;
    GC gc = xgc_;
    XPoint* xp = point_;
    if (n == 5 && cur_subpath_ == subpath_ && rectangular(xp)) {
	int x = Math::min(xp[0].x, xp[2].x);
	int y = Math::min(xp[0].y, xp[2].y);
	int w = Math::abs(xp[2].x - xp[0].x);
	int h = Math::abs(xp[2].y - xp[0].y);
	XFillRectangle(dpy, d, gc, x, y, w, h);
    } else {
	XFillPolygon(dpy, d, gc, xp, n, Complex, CoordModeOrigin);
    }
}

//+ XPainterImpl(Painter::stroke)
void XPainterImpl::stroke() {
    XDisplay* dpy = xdisplay_;
    XDrawable d = xdrawable_;
    GC gc = xgc_;
    for (SubPathInfo* sp = subpath_; sp <= cur_subpath_; sp++) {
	int n = sp->end - sp->start;
	if (n <= 0) {
	    /* not terminated, stroke to the end */
	    n = int((cur_point_ - point_) + sp->start);
	}
	XPoint* xp = point_ + sp->start;
	if (n == 2) {
	    XDrawLine(dpy, d, gc, xp[0].x, xp[0].y, xp[1].x, xp[1].y);
	} else if (n == 5 && rectangular(xp)) {
	    int x = Math::min(xp[0].x, xp[2].x);
	    int y = Math::min(xp[0].y, xp[2].y);
	    int w = Math::abs(xp[2].x - xp[0].x);
	    int h = Math::abs(xp[2].y - xp[0].y);
	    XDrawRectangle(dpy, d, gc, x, y, w, h);
	} else if (n > 2) {
	    XDrawLines(dpy, d, gc, xp, n, CoordModeOrigin);
	}
    }
}

//+ XPainterImpl(Painter::character)
void XPainterImpl::character(CharCode ch, Coord width, Coord x, Coord y) {
    FontInfo* i = fontinfo_;
    if (matrix_modified_) {
	matrix_modified_ = false;
	matrix_tag_ = matrix_->attach(this);
	if (is_not_nil(i->transform) &&
	    translated_only(matrix_, i->transform)
	) {
	    translate_remainder(matrix_, i->remainder);
	} else {
	    set_font();
	    i = fontinfo_;
	}
    }
    Boolean translation_only = is_translation(i->remainder);
    if (!translation_only) {
	adjust_for_remainder(i->remainder, matrix_, x, y);
	FontImpl* font = is_not_nil(i->surrogate) ? i->surrogate : i->font;
	stencil_with_transform(font->bitmap(ch), x, y, i->remainder);
	return;
    }

    Coord sx = x, swidth = width;
    if (is_not_nil(i->surrogate)) {
	Coord swidth = i->widths[ch];
	if (Math::equal(swidth, float(0.0), float(1e-3))) {
	    Font::Info info;
	    i->surrogate->char_info(ch, info);
	    swidth = info.width;
	    i->widths[ch] = swidth;
	}
    }
	    
    if (translation_only) {
	Vertex v;
	v.x = x;
	v.y = y;
	v.z = 0.0;
	i->remainder->transform_vertex(v);
	sx = v.x;
	y = v.y;
    }

    if (!Math::equal(y, ty_, float(1e-2)) ||
	char_ + fontinfo_->charsize >= array_end(chars_)
    ) {
	flush_text();
    }

    if (char_ == chars_) {
	tx0_ = inline_to_pixels(sx);
	ty0_ = pheight_ - inline_to_pixels(y);
	tx_ = x;
	ty_ = y;
	tsx_ = sx;
    } else if (!Math::equal(x, tx_, Coord(1e-3))) {
	PixelCoord psx = inline_to_pixels(sx);
	PixelCoord ptsx = inline_to_pixels(tsx_);
	add_item();
	item_->delta = XCoord(psx - ptsx);
    }
    item_->nchars += fontinfo_->charsize;
    tx_ = x + width;
    tsx_ = sx + swidth;
    add_char(ch);
}

//+ XPainterImpl(Painter::image)
void XPainterImpl::image(Raster_in r, Coord x, Coord y) {
    RasterImpl* raster = RasterImpl::_narrow(r);
    if (raster == nil) {
	/*
	 * Not a known kind of image -- should extract colors
	 * with peek and build a RasterImpl.
	 */
	return;
    }

    // Before calling RasterImpl::lookup, we remove the translation
    // part of the transform. We'll add it afterwards.
    Vertex v;
    v.x = 0; v.y = 0; v.z = 0;
    matrix_->transform_vertex(v);
    TransformRef t;
    if (v.x == 0 && v.y == 0 && v.z == 0) {
	Fresco::ref(matrix_);
	t = matrix_;
    } else {
	Transform::Matrix m;
	matrix_->store_matrix(m);
	t = new TransformImpl(m);
	v.x = - v.x;
	v.y = - v.y;
	v.z = - v.z;
	t->translate(v);
    }

    RasterImpl::PerScreenData* psd = raster->lookup(window_, t);
    if (psd == nil || psd->pixmap == nil) {
	return;
    }
    XDisplay* dpy = xdisplay_;

    v.x = x; v.y = y; v.z = 0;
    matrix_->transform_vertex(v);

    PixelCoord px, py;
    px = Math::round(inline_to_pixels(v.x) + psd->origin_x);
    py = Math::round(pheight_ - inline_to_pixels(v.y) + psd->origin_y);

    /*
     * We assume that graphics exposures are off in the gc.
     */
    XCopyArea(
	dpy, psd->pixmap, xdrawable_, xgc_,
	0, 0, psd->pwidth, psd->pheight, int(px), int(py)
    );
}

/*
 * Stencil fills through a given bitmap, aligning its origin
 * to the given position.
 *
 * We split this into two functions so that we can pass a special
 * transform for when we are stenciling characters.  Otherwise,
 * we'd be modifying the transform for stenciling, which in turn
 * would trigger update notification from the matrix.
 */

//+ XPainterImpl(Painter::stencil)
void XPainterImpl::stencil(Raster_in r, Coord x, Coord y) {
    stencil_with_transform(r, x, y, matrix_);
}

void XPainterImpl::stencil_with_transform(
    Raster_in r, Coord x, Coord y, Transform_in tx
) {
    RasterImpl* raster = RasterImpl::_narrow(r);
    if (raster == nil) {
	/*
	 * Not a known kind of image -- should extract colors
	 * with peek and build a RasterImpl.
	 */
	return;
    }

    // Before calling RasterImpl::lookup, we remove the translation
    // part of the transform. We'll add it afterwards.
    Vertex v;
    v.x = 0; v.y = 0; v.z = 0;
    tx->transform_vertex(v);
    TransformRef t;
    if (v.x == 0 && v.y == 0 && v.z == 0) {
	t = tx;
    } else {
	Transform::Matrix m;
	tx->store_matrix(m);
	t = new TransformImpl(m);
	v.x = - v.x;
	v.y = - v.y;
	v.z = - v.z;
	t->translate(v);
    }

    RasterImpl::PerScreenData* psd = raster->lookup(window_, t);
    if (psd == nil || psd->pixmap == nil) {
	return;
    }
    XDisplay* dpy = xdisplay_;

    v.x = x; v.y = y; v.z = 0;
    tx->transform_vertex(v);

    PixelCoord px = Math::round(inline_to_pixels(v.x) + psd->origin_x);
    PixelCoord py = Math::round(
	pheight_ - inline_to_pixels(v.y) + psd->origin_y
    );

    XGCValues gcv;
    ULong valuemask = 0;

    valuemask |= GCFunction;
    gcv.function = GXand;
    valuemask |= GCForeground;
    gcv.foreground = 0;
    valuemask |= GCBackground;
    gcv.background = AllPlanes;
    valuemask |= GCGraphicsExposures;
    gcv.graphics_exposures = False;

    XDrawable d = xdrawable_;
    GC xgc = XCreateGC(dpy, d, valuemask, &gcv);
    XCopyGC(dpy, xgc_, GCClipMask, xgc);

    XCopyPlane(
	dpy, psd->pixmap, d, xgc,
	0, 0, psd->pwidth, psd->pheight, XCoord(px), XCoord(py), 1
    );

    gcv.function = GXxor;
    gcv.background = 0;
    valuemask &= ~GCGraphicsExposures;
    XChangeGC(dpy, xgc, valuemask, &gcv);
    XCopyGC(dpy, xgc_, GCForeground, xgc);
    XCopyPlane(
	dpy, psd->pixmap, d, xgc,
	0, 0, psd->pwidth, psd->pheight, int(px), int(py), 1
    );
    XFreeGC(dpy, xgc);
}

static inline void restrict(PixelCoord& c, PixelCoord a, PixelCoord b) {
    if (c < a) {
	c = a;
    } else if (c > b) {
	c = b;
    }
}

void XPainterImpl::set_clip() {
    PixelCoord left = inline_to_pixels(clipping_->lower_.x);
    PixelCoord bottom = inline_to_pixels(clipping_->lower_.y);
    PixelCoord right = inline_to_pixels(clipping_->upper_.x);
    PixelCoord top = inline_to_pixels(clipping_->upper_.y);
    restrict(left, 0, pwidth_);
    restrict(bottom, 0, pheight_);
    restrict(right, 0, pwidth_);
    restrict(top, 0, pheight_);
    xclip_.x = XCoord(left);
    xclip_.y = XCoord(pheight_ - top);
    xclip_.width = XCoord(right - left);
    xclip_.height = XCoord(top - bottom);
    XSetClipRectangles(xdisplay_, xgc_, 0, 0, &xclip_, 1, YXBanded);
}

void XPainterImpl::reset_clip() {
    if (clipping_ == nil) {
	XSetClipMask(xdisplay_, xgc_, None);
    } else {
	set_clip();
    }
}

/*
 * Prepare the painter for X drawing by allocating a graphics context.
 */

void XPainterImpl::prepare(Boolean double_buffered) {
    cleanup();
    double_buffered_ = double_buffered;
    XGCValues gcv;
    gcv.graphics_exposures = False;
    gcv.fill_style = FillSolid;
    xfrontbuffer_ = window_->xwindow();
    if (double_buffered) {
	xfrontgc_ = XCreateGC(
	    xdisplay_, xfrontbuffer_, GCGraphicsExposures, &gcv
	);
	xbackbuffer_ = XCreatePixmap(
	    xdisplay_, xfrontbuffer_,
	    XCoord(window_->pwidth()), XCoord(window_->pheight()),
	    int(window_->visual()->depth)
	);
	xdrawable_ = xbackbuffer_;
    } else {
	xfrontbuffer_ = window_->xwindow();
	xfrontgc_ = nil;
	xbackbuffer_ = nil;
	xdrawable_ = xfrontbuffer_;
    }
    xgc_ = XCreateGC(
	xdisplay_, xdrawable_, GCGraphicsExposures | GCFillStyle, &gcv
    );
    pwidth_ = window_->pwidth();
    pheight_ = window_->pheight();
    Fresco::unref(brush_);
    brush_ = nil;
    Fresco::unref(color_);
    color_ = nil;
    clear_font(fontinfo_);
}

void XPainterImpl::frontbuffer() {
    xdrawable_ = xfrontbuffer_;
}

void XPainterImpl::backbuffer() {
    if (double_buffered_) {
	xdrawable_ = xbackbuffer_;
    }
}

void XPainterImpl::swapbuffers() {
    flush_text();
    if (double_buffered_) {
	XCopyArea(
	    xdisplay_, xbackbuffer_, xfrontbuffer_, xfrontgc_,
	    xclip_.x, xclip_.y, xclip_.width, xclip_.height, xclip_.x, xclip_.y
	);
    }
}

void XPainterImpl::init_fonts() {
    FontInfo* i;
    for (i = fontinfos_; i < array_end(fontinfos_); i++) {
	i->font = nil;
	i->transform = nil;
	i->surrogate = nil;
	i->widths = nil;
	i->remainder = nil;
	i->xfont = None;
	i->charsize = 0;
	i->age = 0;
    }
    fontinfo_ = fontinfos_;
}

void XPainterImpl::init_items() {
    char_ = chars_;
    item_ = items_;
    item_->chars = chars_;
    item_->nchars = 0;
    item_->delta = 0;
    item_->font = None;
}

XPainterImpl::FontInfo* XPainterImpl::open_font(FontRef f) {
    FontInfo* i = find_font(f);
    if (i->font != nil) {
	translate_remainder(matrix_, i->remainder);
	return i;
    }
    /*
     * Should use narrow
     */
    Fresco::ref(f);
    FontImpl* fi = (FontImpl*)f;
    i->font = fi;
    XFontStruct* xf = fi->xfont();
    if (xf->min_byte1 > 0 || xf->max_byte1 > 0) {
	i->charsize = 2;
    } else {
	i->charsize = 1;
    }
    TransformImpl* t = new TransformImpl;
    t->load(matrix_);
    i->transform = t;
    i->surrogate = substitute(fi, scale(t, Y_axis));
    if (is_nil(i->surrogate)) {
	i->xfont = xf->fid;
    } else {
	FontImpl* s = i->surrogate;
	s->load();
	XFontStruct* xfont = s->xfont();
	i->xfont = xfont->fid;
	Long max = xfont->max_char_or_byte2 + 1;
	Coord* widths = new Coord[max];
	for (Long w = 0; w < max; w++) {
	    widths[w] = 0.0;
	}
	i->widths = widths;
    }
    i->remainder = remainder(fi, t, i->surrogate);
    return i;
}

XPainterImpl::FontInfo* XPainterImpl::find_font(FontRef f) {
    FontInfo* i = nil;
    FontInfo* oldest = nil;
    FontInfo* cur = fontinfo_;
    for (;;) {
	++cur;
	if (cur >= array_end(fontinfos_)) {
	    cur = fontinfos_;
	}
	if (cur == fontinfo_) {
	    break;
	}
	if (i == nil) {
	    if (is_nil(cur->font)) {
		oldest = cur;
	    } else {
		if (f->equal(cur->font) &&
		    translated_only(matrix_, cur->transform)
		) {
		    i = cur;
		} else if (
		    oldest == nil || (
			is_not_nil(oldest->font) && cur->age > oldest->age
		    )
		) {
		    oldest = cur;
		}
	    }
	}
	cur->age += 1;
    }
    if (i == nil) {
	i = oldest;
	if (i->font != nil) {
	    clear_font(i);
	}
    }
    return i;
}

void XPainterImpl::close_fonts() {
    FontInfo* i;
    for (i = fontinfos_; i < array_end(fontinfos_); i++) {
	if (xdisplay_ != nil &&
	    i->font != nil && i->font->xdisplay() != xdisplay_
	) {
	    XUnloadFont(xdisplay_, i->xfont);
	}
	i->xfont = nil;
    }
}

void XPainterImpl::cleanup() {
    if (xdisplay_ != nil) {
	if (xgc_ != nil) {
	    XFreeGC(xdisplay_, xgc_);
	}
	if (double_buffered_) {
	    if (xfrontgc_ != nil) {
		XFreeGC(xdisplay_, xfrontgc_);
	    }
	    if (xbackbuffer_ != nil) {
		XFreePixmap(xdisplay_, xbackbuffer_);
	    }
	}
    }
}

void XPainterImpl::clear_font(FontInfo* i) {
    Fresco::unref(i->font);
    i->font = nil;
    Fresco::unref(i->transform);
    i->transform = nil;
    Fresco::unref(i->surrogate);
    i->surrogate = nil;
    delete [] i->widths;
    i->widths = nil;
    Fresco::unref(i->remainder);
    i->remainder = nil;
}

XPoint* XPainterImpl::next_point() {
    if (cur_point_ == end_point_) {
	Long old_size = cur_point_ - point_;
	Long new_size = old_size + old_size;
	XPoint* new_path = new XPoint[new_size];
	for (Long i = 0; i < old_size; i++) {
	    new_path[i] = point_[i];
	}
	delete point_;
	point_ = new_path;
	cur_point_ = point_ + old_size;
	end_point_ = point_ + new_size;
    }
    XPoint* xp = cur_point_;
    cur_point_ = xp + 1;
    return xp;
}

XPainterImpl::SubPathInfo* XPainterImpl::next_subpath() {
    ++cur_subpath_;
    if (cur_subpath_ == end_subpath_) {
	Long old_size = cur_subpath_ - subpath_;
	Long new_size = old_size + old_size;
	SubPathInfo* new_subpath = new SubPathInfo[new_size];
	for (Long i = 0; i < old_size; i++) {
	    new_subpath[i] = subpath_[i];
	}
	delete subpath_;
	subpath_ = new_subpath;
	cur_subpath_ = subpath_ + old_size;
	end_subpath_ = subpath_ + new_size;
    }
    return cur_subpath_;
}

Boolean XPainterImpl::rectangular(XPoint* p) {
    int x0 = p[0].x, y0 = p[0].y;
    if (x0 == p[4].x && y0 == p[4].y) {
	int x1 = p[1].x, y1 = p[1].y;
	int x2 = p[2].x, y2 = p[2].y;
	int x3 = p[3].x, y3 = p[3].y;
	return (
	    (x0 == x1 && y1 == y2 && x2 == x3 && y3 == y0) ||
	    (x0 == x3 && y3 == y2 && x2 == x1 && y1 == y0)
	);
    }
    return false;
}

void XPainterImpl::curve_pt(
    Coord px, Coord py, Coord x, Coord y,
    Coord x1, Coord y1, Coord x2, Coord y2
) {
    if (straight(px, py, x1, y1, x2, y2, x, y)) {
	XPoint* xp = next_point();
	xp->x = short(inline_to_pixels(x));
	xp->y = short(pheight_ - inline_to_pixels(y));
    } else {
	Coord xx = mid(x1, x2);
	Coord yy = mid(y1, y2);
	Coord x11 = mid(px, x1);
	Coord y11 = mid(py, y1);
	Coord x22 = mid(x2, x);
	Coord y22 = mid(y2, y);
	Coord x12 = mid(x11, xx);
	Coord y12 = mid(y11, yy);
	Coord x21 = mid(xx, x22);
	Coord y21 = mid(yy, y22);
	Coord cx = mid(x12, x21);
	Coord cy = mid(y12, y21);

	curve_pt(px, py, cx, cy, x11, y11, x12, y12);
	curve_pt(cx, cy, x, y, x21, y21, x22, y22);
    }
}

/*
 * Allocate another XTextItem in the text buffer.
 */

void XPainterImpl::add_item() {
    XTextItem* i = item_;
    if (i->nchars != 0) {
	++i;
	if (i >= array_end(items_)) {
	    flush_text();
	    i = item_;
	} else {
	    item_ = i;
	}
	i->chars = char_;
	i->nchars = 0;
	i->delta = 0;
	i->font = None;
    }
}

/*
 * Add a character code into the text buffer.
 */

void XPainterImpl::add_char(CharCode ch) {
    char* cp = char_;
    switch (fontinfo_->charsize) {
    case 4:
	*cp++ = char((ch & 0xff000000) >> 24);
	/* goto case 3 */
    case 3:
	*cp++ = char((ch & 0xff0000) >> 16);
	/* goto case 2 */
    case 2:
	*cp++ = char((ch & 0xff00) >> 8);
	/* goto case 1 */
    case 1:
	*cp++ = char(ch & 0xff);
    }
    char_ = cp;
}

/*
 * Force drawing of any buffered text.
 */

void XPainterImpl::flush_text() {
    if (char_ == chars_) {
	return;
    }
    int n = item_ - items_ + 1;
    XDisplay* dpy = xdisplay_;
    XDrawable d = xdrawable_;
    GC gc = xgc_;
    XCoord x = XCoord(tx0_);
    XCoord y = XCoord(ty0_);
    switch (fontinfo_->charsize) {
    case 1:
	XDrawText(dpy, d, gc, x, y, items_, n);
	break;
    case 2:
	XDrawText16(dpy, d, gc, x, y, (XTextItem16*)items_, n);
	break;
    }
    init_items();
    item_->font = fontinfo_->xfont;
}

float XPainterImpl::scale(TransformRef t, Axis a) {
    Vertex v0, v1;
    v0.x = 0;
    v0.y = 0;
    switch (a) {
    case X_axis:
	v1.x = 1;
	v1.y = 0;
	break;
    case Y_axis:
	v1.x = 0;
	v1.y = 1;
	break;
    case Z_axis:
	return 0.0;
    }
    t->transform_vertex(v0);
    t->transform_vertex(v1);
    Coord dx = v1.x - v0.x;
    Coord dy = v1.y - v0.y;
    return sqrt(dx * dx + dy * dy);
}

/*
 *  Return the best font that accounts for the difference in dpi between the
 *  given font and this screen and the scaling of the transformation.  A nil
 *  return value means that the given font should be used.
 */
FontImpl* XPainterImpl::substitute(FontImpl* f, float scale) {
    FontImpl* result = nil;
    CharStringBuffer name(_tmp(f->name()));
    if (FontSpec::valid(name.string())) {
	FontSpec spec(name.string());
	if (spec.is_scalable()) {
	    result = scaled(f, spec, scale);
	} else {
	    result = closest(f, spec, scale);
	}
    }
    return result;
}

FontImpl* XPainterImpl::scaled(FontImpl* f, FontSpec& spec, float scale) {
    FontImpl* result = nil;
    PixelCoord dpi = PixelCoord(screen_->dpi());
    float rscale =  float(dpi) / atol(spec.field(FontSpec::ydpi));
    if (!Math::equal(rscale, 1.0f, 0.05f) || scale != 1.0) {
	scale *= rscale;
	Coord desired_width = f->average_width() * scale;
	PixelCoord points = atoi(spec.field(FontSpec::points));
	if (points == 0) {
	    Font::Info info;
	    f->font_info(info);
	    points = to_pixels(info.height) * 10;
	    char value[5];
	    sprintf(value, "%d", points);
	    spec.field(FontSpec::points, value);
	}
	for (;;) {
	    Boolean zero = scale_field(spec, scale, FontSpec::points, false);
	    zero = scale_field(spec, scale, FontSpec::pixels, zero);
	    zero = scale_field(spec, scale, FontSpec::width, zero);
	    result = new FontImpl(window_->display(), spec.full_spec());
	    Coord average_width = result->average_width();
	    if (average_width <= desired_width) {
		break;
	    } else {
		scale = desired_width/average_width;
		Fresco::unref(result);
		result = nil;
	    }
      }
    }
    return result;
}

/*
 *  The fields pixels, points, and widths are all scalable.  If we scale all
 *  fields together, the values may not be satisfied simultaneously by the
 *  server.  Instead, we only scale one field and zero the other two.
 *
 *  A scalable font specification should be like:
 *
 *    pixel:          0
 *    points:         specify
 *    [x,y]dpi:       match current display or existing font (xlsfonts)+
 *    width:          0
 *
 *  + if we are on a 90 dpi screen, and xlsfonts show that scalable fonts
 *    exist for 72, 75, or 100 dpi for the desired font family.  Then the
 *    dpi field could be 72, 75, 90, or 100.  90 would be most efficient,
 *    but not portable.
 */
Boolean XPainterImpl::scale_field(
    FontSpec& spec, float scale, unsigned field, Boolean zero
) {
    PixelCoord pvalue = 0;
    char buffer[10];
    if (!zero) {
	XCoord xvalue = atoi(spec.field(field));
	if (xvalue != 0) {
	    pvalue = PixelCoord(xvalue * scale);
	    zero = true;
	}
    }
    sprintf(buffer, "%d", pvalue);
    spec.field(field, buffer);
    return zero;
}

/*
 *  Return a bitmapped font that best matches the height
 *  requirement scaled by the transformation.
 */
FontImpl* XPainterImpl::closest(FontImpl* f, FontSpec& spec, float scale) {
    FontImpl* result = nil;
    char** family;
    Long count = font_family(spec, family);
    if (count > 0) {
	Font::Info info;
	f->font_info(info);
	Coord width = f->to_coord(atoi(spec.field(FontSpec::width)));
	Long pixel_height = inline_to_pixels(info.height * scale);
	Long pixel_width = inline_to_pixels(width * scale);
	FontSpec best;
	Long best_height = 0;
	Long best_width = 0;
	for (Long i = 0; i < count; i++) {
	    FontSpec spec(family[i]);
	    Long p = atol(spec.field(FontSpec::pixels));
	    Long w = atol(spec.field(FontSpec::width));
	    if (p >= best_height && p <= pixel_height && w <= pixel_width) {
		if (p > best_height || (p == best_height && w > best_width)) {
		    best = spec;
		    best_height = p;
		    best_width = w;
		}
	    }
	}
	if (best_height != 0) {
	    result = new FontImpl(window_->display(), best.full_spec());
	}
	XFreeFontNames(family);
    }
    return result;
}

Long XPainterImpl::font_family(FontSpec& given, char**& names) {
    FontSpec spec(given);
    spec.field(FontSpec::pixels, "*");
    spec.field(FontSpec::points, "*");
    spec.field(FontSpec::xdpi, "*");
    spec.field(FontSpec::ydpi, "*");
    spec.field(FontSpec::width, "*");
    const int max_names = 10000;
    int count;
    names = XListFonts(xdisplay_, spec.full_spec(), max_names, &count);
    if (names == NULL) {
	count = 0;
    }
    return count;
}

TransformImpl* XPainterImpl::remainder(
    FontImpl* font, TransformImpl* t, FontImpl* surrogate
) {
    TransformImpl* result = new TransformImpl;
    result->load(t);
    if (is_not_nil(surrogate)) {
	Font::Info fi, si;
	font->font_info(fi);
	surrogate->font_info(si);
	float y_scale = scale(t, Y_axis);
	float f_scale = fi.height / si.height;
	if (!Math::equal(f_scale * y_scale, 1.0f, font_tol_) ||
	    !Math::equal(scale(t, X_axis), y_scale, 0.01f)
	) {
	    Vertex vt, v;
	    vt.x = 0;
	    vt.y = 0;
	    vt.z = 0;
	    result->transform_vertex(vt);
	    vt.x = -vt.x;
	    vt.y = -vt.y;
	    vt.z = -vt.z;
	    result->translate(vt);

	    v.x = f_scale;
	    v.y = f_scale;
	    v.z = 0;
	    result->scale(v);

	    vt.x = -vt.x;
	    vt.y = -vt.y;
	    vt.z = -vt.z;
	    result->translate(vt);
	}
    }
    return result;
}

void XPainterImpl::adjust_for_remainder(
    TransformImpl* remainder, TransformImpl* painter, Coord& x, Coord& y
) {
    if (!remainder->equal(painter)) {
	Vertex v;
	v.x = x;
	v.y = y;
	v.z = 0;
	painter->transform_vertex(v);
	remainder->inverse_transform_vertex(v);
	x = v.x;
	y = v.y;
    }
}

/* class FontSpec */

FontSpec::FontSpec(const char* full_spec) {
    copy(full_spec);
}

FontSpec::FontSpec(const FontSpec& other) {
    copy(other);
}

FontSpec::FontSpec() {
    for (Long i = 0; i < max_index; i++) {
	fields_[i] = nil;
    }
    updated_ = true;
}

FontSpec::~FontSpec() {
    for (Long i = 0; i < max_index; i++) {
	delete fields_[i];
    }
}

void FontSpec::operator=(const FontSpec& other) {
    for (int i = 0; i < max_index; i++) {
	delete fields_[i];
    }
    copy(other);
}

const char* FontSpec::full_spec() const {
    if (updated_) {
	FontSpec* self = (FontSpec*)this;
	self->update();
	self->updated_ = false;
    }
    return specs_;
}

Boolean FontSpec::is_scalable() {
    return (
	atoi(fields_[pixels]) == 0 ||
	atoi(fields_[points]) == 0 || atoi(fields_[width]) == 0
    );
}

char* FontSpec::field(FontSpecIndex index) const { return fields_[index]; }

void FontSpec::field(FontSpecIndex index, char* field) {
    delete fields_[index];
    fields_[index] = new char[strlen(field) + 1];
    strcpy(fields_[index], field);
    updated_ = true;
}

Boolean FontSpec::valid(const char* name) {
    int field = 0;
    for (int i = 0; name[i] != '\0'; i++) {
	if (name[i] == '-') {
	    ++field;
	}
    }
    return field == 14;
}

void FontSpec::copy(const FontSpec& other) {
    for (int i = 0; i < max_index; i++) {
	const char* field = other.fields_[i];
	if (field != nil) {
	    fields_[i] = new char[strlen(field) + 1];
	    strcpy(fields_[i], field);
	} else {
	    fields_[i] = nil;
	}
    }
    updated_ = true;
}

void FontSpec::copy(const char* name) {
    const char* p = name;
    for (int i = 0; i < max_index; i++) {
	++p;
	const char* field = p;
	while (*p != nil && *p != '-') {
	    ++p;
	}
	int length = p - field;
	if (length == 0 || (length == 1 && *field == '*')) {
	    fields_[i] = nil;
	} else {
	    char* f = new char[length + 1];
	    strncpy(f, field, length);
	    f[length] = 0;
	    fields_[i] = f;
	}
    }
    updated_ = true;
}

void FontSpec::update() {
    char* specs = &specs_[0];
    for (int i = 0; i < max_index; i++) {
	*specs++ = '-';
	const char* field = fields_[i];
	if (field != 0) {
	    int length = strlen(field);
	    strcpy(specs, field);
	    specs += length;
	}
    }
    *specs = '\0';
}

/*
 * DrawingKit -- create drawing objects
 */

declareTable(DrawingKitColorTable,CharStringRef,ColorRef)
implementTable(DrawingKitColorTable,CharStringRef,ColorRef)

declareTable(DrawingKitFontTable,CharStringRef,FontRef)
implementTable(DrawingKitFontTable,CharStringRef,FontRef)

DrawingKitImpl::DrawingKitImpl(DisplayImpl* d) {
    Fresco::ref(d);
    display_ = d;
    colors_ = new DrawingKitColorTable(20);
    fonts_ = new DrawingKitFontTable(20);
    foreground_str_ = Fresco::string_ref("foreground");
    Foreground_str_ = Fresco::string_ref("Foreground");
    background_str_ = Fresco::string_ref("background");
    Background_str_ = Fresco::string_ref("Background");
    font_str_ = Fresco::string_ref("font");
    Font_str_ = Fresco::string_ref("Font");
}

DrawingKitImpl::~DrawingKitImpl() {
    Fresco::unref(display_);
    delete colors_;
    delete fonts_;
}

//+ DrawingKitImpl(FrescoObject::=object_.)
Long DrawingKitImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag DrawingKitImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void DrawingKitImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void DrawingKitImpl::disconnect() {
    object_.disconnect();
}
void DrawingKitImpl::notify_observers() {
    object_.notify_observers();
}
void DrawingKitImpl::update() {
    object_.update();
}
//+

//+ DrawingKitImpl(DrawingKit::drawing_style)
Style_return DrawingKitImpl::drawing_style() {
    return display_->display_style();
}

//+ DrawingKitImpl(DrawingKit::simple_brush)
Brush_return DrawingKitImpl::simple_brush(Coord width) {
    return new BrushImpl(width);
}

//+ DrawingKitImpl(DrawingKit::dither_brush)
Brush_return DrawingKitImpl::dither_brush(Coord width, Long p) {
    return new BrushImpl(p, width);
}

//+ DrawingKitImpl(DrawingKit::patterned_brush)
Brush_return DrawingKitImpl::patterned_brush(Coord width, const Data32& p) {
    return new BrushImpl(p._buffer, p._length, width);
}

//+ DrawingKitImpl(DrawingKit::solid_pattern)
Pattern_return DrawingKitImpl::solid_pattern() {
    return new PatternImpl(0xffff);
}

//+ DrawingKitImpl(DrawingKit::halftone_pattern)
Pattern_return DrawingKitImpl::halftone_pattern() {
    return new PatternImpl(0xa5a5);
}

//+ DrawingKitImpl(DrawingKit::stipple)
Pattern_return DrawingKitImpl::stipple(const Data32& d) {
    return new PatternImpl(d);
}

//+ DrawingKitImpl(DrawingKit::stipple_4x4)
Pattern_return DrawingKitImpl::stipple_4x4(ULong p) {
    return new PatternImpl(p);
}

//+ DrawingKitImpl(DrawingKit::color_rgb)
Color_return DrawingKitImpl::color_rgb(Color::Intensity r, Color::Intensity g, Color::Intensity b) {
    return new ColorImpl(r, g, b);
}

//+ DrawingKitImpl(DrawingKit::find_color)
Color_return DrawingKitImpl::find_color(CharString_in name) {
    ColorRef c = nil;
    if (!colors_->find(c, name)) {
	XColor xc;
	CharStringBuffer cs(name);
	XDisplay* dpy = display_->xdisplay();
	if (XParseColor(
	    dpy, DefaultColormap(dpy, DefaultScreen(dpy)), cs.string(), &xc
	)) {
	    float range = float(0xffff);
	    Color::Intensity r = float(xc.red) / range;
	    Color::Intensity g = float(xc.green) / range;
	    Color::Intensity b = float(xc.blue) / range;
	    c = new ColorImpl(r, g, b);
	    colors_->insert(name, c);
	}
    }
    return Color::_duplicate(c);
}

//+ DrawingKitImpl(DrawingKit::resolve_color)
Color_return DrawingKitImpl::resolve_color(Style_in s, CharString_in name) {
    Color_return c = nil;
    StyleValue_var a = s->resolve(name);
    if (is_not_nil(a)) {
	CharString_var v;
	if (a->read_string(v._out())) {
	    c = find_color(v);
	}
    }
    return c;
}

//+ DrawingKitImpl(DrawingKit::foreground)
Color_return DrawingKitImpl::foreground(Style_in s) {
    return resolve_colors(s, foreground_str_, Foreground_str_);
}

//+ DrawingKitImpl(DrawingKit::background)
Color_return DrawingKitImpl::background(Style_in s) {
    return resolve_colors(s, background_str_, Background_str_);
}

ColorRef DrawingKitImpl::resolve_colors(
    StyleRef s, CharString_in name1, CharString_in name2
) {
    Color_return c = resolve_color(s, name1);
    if (is_nil(c)) {
	c = resolve_color(s, name2);
    }
    return c;
}

//+ DrawingKitImpl(DrawingKit::find_font)
Font_return DrawingKitImpl::find_font(CharString_in fullname) {
    FontRef f = nil;
    if (!fonts_->find(f, fullname)) {
	CharStringBuffer cs(fullname);
	const char* lfd = cs.string();
	int n = 0;
	char** names = nil;
	if (FontSpec::valid(lfd)) {
	    FontSpec spec(lfd);
	    if (spec.is_scalable()) {
		spec.field(FontSpec::pixels, "0");
		spec.field(FontSpec::points, "0");
		spec.field(FontSpec::width, "0");
		names = XListFonts(
		    display_->xdisplay(), spec.full_spec(), 1, &n
		);
		if (n == 1) {
		    f = new FontImpl(display_, lfd);
		    fonts_->insert(fullname, f);
		}
	    }
	}
	if (n == 0) {
	    names = XListFonts(display_->xdisplay(), lfd, 1, &n);
	    if (n == 1) {
		f = new FontImpl(display_, names[0]);
		fonts_->insert(fullname, f);
	    }
	}
	if (n > 0) {
	    XFreeFontNames(names);
	}
    }
    return Font::_duplicate(f);
}

//+ DrawingKitImpl(DrawingKit::find_font_match)
Font_return DrawingKitImpl::find_font_match(CharString_in font_family, CharString_in font_style, Coord ptsize) {
    return nil;
}

//+ DrawingKitImpl(DrawingKit::resolve_font)
Font_return DrawingKitImpl::resolve_font(Style_in s, CharString_in name) {
    Font_return f = nil;
    StyleValue_var a = s->resolve(name);
    if (is_not_nil(a)) {
	CharString_var v;
	if (a->read_string(v._out())) {
	    f = find_font(v);
	}
    }
    return f;
}

//+ DrawingKitImpl(DrawingKit::default_font)
Font_return DrawingKitImpl::default_font(Style_in s) {
    Font_return f = resolve_font(s, font_str_);
    if (is_nil(f)) {
	f = resolve_font(s, Font_str_);
    }
    return f;
}

//+ DrawingKitImpl(DrawingKit::raster_open)
Raster_return DrawingKitImpl::raster_open(CharString_in name) {
    if (is_nil(name)) {
	return nil;
    }
    CharStringBuffer cs(name);
    unsigned int width, height;
    int x, y;
    Pixmap pixmap;

    if (BitmapOpenFailed == XReadBitmapFile(
	display_->xdisplay(), DefaultRootWindow(display_->xdisplay()),
	cs.string(), &width, &height, &pixmap, &x, &y
    )) {
	return nil;
    }
    if (x == -1) {
	x = 0;
    }
    if (y == -1) {
	y = height;
    }
    DrawingKit::Data8 data;
    data._buffer = nil;
    RasterBitmap* rb = new RasterBitmap(data, height, width, height - y, x, 0);
    rb->read_drawable(display_->xdisplay(), pixmap, 0, 0);
    XFreePixmap(display_->xdisplay(), pixmap);
    return rb;
}

//+ DrawingKitImpl(DrawingKit::bitmap_from_data)
Raster_return DrawingKitImpl::bitmap_from_data(const Data8& d, Raster::Index rows, Raster::Index columns, Raster::Index origin_row, Raster::Index origin_column) {
    return new RasterBitmap(d, rows, columns, origin_row, origin_column);
}

//+ DrawingKitImpl(DrawingKit::bitmap_from_char)
Raster_return DrawingKitImpl::bitmap_from_char(Font_in f, CharCode c) {
    FontImpl* fi = (FontImpl*)f;
    return fi->bitmap(c);
}

//+ DrawingKitImpl(DrawingKit::transform_matrix)
Transform_return DrawingKitImpl::transform_matrix(Transform::Matrix m) {
    return new TransformImpl(m);
}

//+ DrawingKitImpl(DrawingKit::identity_matrix)
Transform_return DrawingKitImpl::identity_matrix() {
    return new TransformImpl;
}

//+ DrawingKitImpl(DrawingKit::printer)
Painter_return DrawingKitImpl::printer(CharString_in output) {
    return nil;
}
