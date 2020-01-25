/* DO NOT EDIT -- Automatically generated from Interfaces/drawing.idl */

#ifndef Interfaces_drawing_h
#define Interfaces_drawing_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/types.h>

class Brush;
typedef Brush* BrushRef;
typedef BrushRef Brush_in;
typedef BrushRef Brush_out, Brush_inout;
typedef BrushRef Brush_return;
class Brush_var;

class Font;
typedef Font* FontRef;
typedef FontRef Font_in;
typedef FontRef Font_out, Font_inout;
typedef FontRef Font_return;
class Font_var;

class Pattern;
typedef Pattern* PatternRef;
typedef PatternRef Pattern_in;
typedef PatternRef Pattern_out, Pattern_inout;
typedef PatternRef Pattern_return;
class Pattern_var;

class Raster;
typedef Raster* RasterRef;
typedef RasterRef Raster_in;
typedef RasterRef Raster_out, Raster_inout;
typedef RasterRef Raster_return;
class Raster_var;

class Color;
typedef Color* ColorRef;
typedef ColorRef Color_in;
typedef ColorRef Color_out, Color_inout;
typedef ColorRef Color_return;
class Color_var;

extern BaseObjectRef _XfColorStub_create(Exchange*);

class Color : public FrescoObject {
protected:
    Color();
    virtual ~Color();
public:
    ColorRef _obj() { return this; };
    static ColorRef  _return_ref(Color_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static ColorRef _narrow(BaseObjectRef);
    static ColorRef _duplicate(ColorRef obj) {
        return (ColorRef)_BaseObject__duplicate(obj, &_XfColorStub_create);
    }
    typedef Float Intensity;
    virtual void rgb(Intensity& r, Intensity& g, Intensity& b);
    virtual Boolean equal(Color_in c);
    virtual ULong hash();
};

class Color_var {
protected:
    ColorRef _obj_;
public:
    Color_var() { _obj_ = 0; };
    Color_var(ColorRef p) { _obj_ = p; }
    Color_var(const Color_var& r) {
        _obj_ = Color::_duplicate(r._obj_);
    }
    ~Color_var() { _BaseObject__release(_obj_); }

    Color_var& operator =(ColorRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Color::_duplicate(p);
        return *this;
    }
    Color_var& operator =(const Color_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Color::_duplicate(r._obj_);
        return *this;
    }

    ColorRef _obj() const { return _obj_; }
    ColorRef& _out() { return _obj_; }
    operator ColorRef() const { return _obj_; }
    ColorRef operator ->() const { return _obj_; }
};

inline Color_var _tmp(ColorRef p) { return Color_var(p); }

class ColorStub : public Color {
public:
    ColorStub(Exchange*);
    ~ColorStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class Painter;
typedef Painter* PainterRef;
typedef PainterRef Painter_in;
typedef PainterRef Painter_out, Painter_inout;
typedef PainterRef Painter_return;
class Painter_var;

extern BaseObjectRef _XfPainterStub_create(Exchange*);

class Painter : public FrescoObject {
protected:
    Painter();
    virtual ~Painter();
public:
    PainterRef _obj() { return this; };
    static PainterRef  _return_ref(Painter_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static PainterRef _narrow(BaseObjectRef);
    static PainterRef _duplicate(PainterRef obj) {
        return (PainterRef)_BaseObject__duplicate(obj, &_XfPainterStub_create);
    }
    virtual Coord to_coord(PixelCoord p);
    virtual PixelCoord to_pixels(Coord c);
    virtual Coord to_pixels_coord(Coord c);
    virtual void begin_path();
    virtual void move_to(Coord x, Coord y);
    virtual void line_to(Coord x, Coord y);
    virtual void curve_to(Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2);
    virtual void close_path();
    virtual Brush_return current_brush();
    virtual void current_brush(Brush_in _p);
    virtual Color_return current_color();
    virtual void current_color(Color_in _p);
    virtual Font_return current_font();
    virtual void current_font(Font_in _p);
    virtual Pattern_return current_pattern();
    virtual void current_pattern(Pattern_in _p);
    virtual void stroke();
    virtual void fill();
    virtual void line(Coord x0, Coord y0, Coord x1, Coord y1);
    virtual void rect(Coord x0, Coord y0, Coord x1, Coord y1);
    virtual void fill_rect(Coord x0, Coord y0, Coord x1, Coord y1);
    virtual void character(CharCode ch, Coord width, Coord x, Coord y);
    virtual void image(Raster_in r, Coord x, Coord y);
    virtual void stencil(Raster_in r, Coord x, Coord y);
    virtual Transform_return current_matrix();
    virtual void current_matrix(Transform_in _p);
    virtual void push_matrix();
    virtual void pop_matrix();
    virtual void premultiply(Transform_in t);
    virtual void clip();
    virtual void clip_rect(Coord x0, Coord y0, Coord x1, Coord y1);
    virtual void push_clipping();
    virtual void pop_clipping();
    virtual Boolean is_visible(Region_in r);
    virtual Region_return visible();
    virtual void comment(CharString_in s);
    virtual void page_number(CharString_in s);
};

class Painter_var {
protected:
    PainterRef _obj_;
public:
    Painter_var() { _obj_ = 0; };
    Painter_var(PainterRef p) { _obj_ = p; }
    Painter_var(const Painter_var& r) {
        _obj_ = Painter::_duplicate(r._obj_);
    }
    ~Painter_var() { _BaseObject__release(_obj_); }

    Painter_var& operator =(PainterRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Painter::_duplicate(p);
        return *this;
    }
    Painter_var& operator =(const Painter_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Painter::_duplicate(r._obj_);
        return *this;
    }

    PainterRef _obj() const { return _obj_; }
    PainterRef& _out() { return _obj_; }
    operator PainterRef() const { return _obj_; }
    PainterRef operator ->() const { return _obj_; }
};

inline Painter_var _tmp(PainterRef p) { return Painter_var(p); }

class PainterStub : public Painter {
public:
    PainterStub(Exchange*);
    ~PainterStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfRasterStub_create(Exchange*);

class Raster : public FrescoObject {
protected:
    Raster();
    virtual ~Raster();
public:
    RasterRef _obj() { return this; };
    static RasterRef  _return_ref(Raster_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static RasterRef _narrow(BaseObjectRef);
    static RasterRef _duplicate(RasterRef obj) {
        return (RasterRef)_BaseObject__duplicate(obj, &_XfRasterStub_create);
    }
    typedef Long Index;
    struct Element {
        Boolean on;
        ColorRef pixel;
        Float blend;
    };
    virtual Boolean equal(Raster_in r);
    virtual ULong hash();
    virtual Index rows();
    virtual Index columns();
    virtual Index origin_x();
    virtual Index origin_y();
    virtual void peek(Index row, Index column, Element& e);
    virtual void poke(Index row, Index column, const Element& e);
    virtual Coord scale();
    virtual void scale(Coord _p);
};

class Raster_var {
protected:
    RasterRef _obj_;
public:
    Raster_var() { _obj_ = 0; };
    Raster_var(RasterRef p) { _obj_ = p; }
    Raster_var(const Raster_var& r) {
        _obj_ = Raster::_duplicate(r._obj_);
    }
    ~Raster_var() { _BaseObject__release(_obj_); }

    Raster_var& operator =(RasterRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Raster::_duplicate(p);
        return *this;
    }
    Raster_var& operator =(const Raster_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Raster::_duplicate(r._obj_);
        return *this;
    }

    RasterRef _obj() const { return _obj_; }
    RasterRef& _out() { return _obj_; }
    operator RasterRef() const { return _obj_; }
    RasterRef operator ->() const { return _obj_; }
};

inline Raster_var _tmp(RasterRef p) { return Raster_var(p); }

class RasterStub : public Raster {
public:
    RasterStub(Exchange*);
    ~RasterStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class DrawingKit;
typedef DrawingKit* DrawingKitRef;
typedef DrawingKitRef DrawingKit_in;
typedef DrawingKitRef DrawingKit_out, DrawingKit_inout;
typedef DrawingKitRef DrawingKit_return;
class DrawingKit_var;

extern BaseObjectRef _XfDrawingKitStub_create(Exchange*);

class DrawingKit : public FrescoObject {
protected:
    DrawingKit();
    virtual ~DrawingKit();
public:
    DrawingKitRef _obj() { return this; };
    static DrawingKitRef  _return_ref(DrawingKit_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static DrawingKitRef _narrow(BaseObjectRef);
    static DrawingKitRef _duplicate(DrawingKitRef obj) {
        return (DrawingKitRef)_BaseObject__duplicate(obj, &_XfDrawingKitStub_create);
    }
    class Data8 {
    public:
        long _maximum, _length; Octet* _buffer;

        Data8() { _maximum = _length = 0; _buffer = 0; }
        Data8(long m, long n, Octet* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        Data8(const Data8& _s) { _buffer = 0; *this = _s; }
        Data8& operator =(const Data8&);
        ~Data8() { delete [] _buffer; }
    };
    class Data32 {
    public:
        long _maximum, _length; Long* _buffer;

        Data32() { _maximum = _length = 0; _buffer = 0; }
        Data32(long m, long n, Long* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        Data32(const Data32& _s) { _buffer = 0; *this = _s; }
        Data32& operator =(const Data32&);
        ~Data32() { delete [] _buffer; }
    };
    virtual Style_return drawing_style();
    virtual Brush_return simple_brush(Coord width);
    virtual Brush_return dither_brush(Coord width, Long p);
    virtual Brush_return patterned_brush(Coord width, const Data32& p);
    virtual Pattern_return solid_pattern();
    virtual Pattern_return halftone_pattern();
    virtual Pattern_return stipple(const Data32& d);
    virtual Pattern_return stipple_4x4(ULong p);
    virtual Color_return color_rgb(Color::Intensity r, Color::Intensity g, Color::Intensity b);
    virtual Color_return find_color(CharString_in name);
    virtual Color_return resolve_color(Style_in s, CharString_in name);
    virtual Color_return foreground(Style_in s);
    virtual Color_return background(Style_in s);
    virtual Font_return find_font(CharString_in fullname);
    virtual Font_return find_font_match(CharString_in font_family, CharString_in font_style, Coord ptsize);
    virtual Font_return resolve_font(Style_in s, CharString_in name);
    virtual Font_return default_font(Style_in s);
    virtual Raster_return bitmap_from_data(const Data8& d, Raster::Index rows, Raster::Index columns, Raster::Index origin_row, Raster::Index origin_column);
    virtual Raster_return bitmap_from_char(Font_in f, CharCode c);
    virtual Raster_return raster_open(CharString_in name);
    virtual Transform_return transform_matrix(Transform::Matrix m);
    virtual Transform_return identity_matrix();
    virtual Painter_return printer(CharString_in output);
};

class DrawingKit_var {
protected:
    DrawingKitRef _obj_;
public:
    DrawingKit_var() { _obj_ = 0; };
    DrawingKit_var(DrawingKitRef p) { _obj_ = p; }
    DrawingKit_var(const DrawingKit_var& r) {
        _obj_ = DrawingKit::_duplicate(r._obj_);
    }
    ~DrawingKit_var() { _BaseObject__release(_obj_); }

    DrawingKit_var& operator =(DrawingKitRef p) {
        _BaseObject__release(_obj_);
        _obj_ = DrawingKit::_duplicate(p);
        return *this;
    }
    DrawingKit_var& operator =(const DrawingKit_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = DrawingKit::_duplicate(r._obj_);
        return *this;
    }

    DrawingKitRef _obj() const { return _obj_; }
    DrawingKitRef& _out() { return _obj_; }
    operator DrawingKitRef() const { return _obj_; }
    DrawingKitRef operator ->() const { return _obj_; }
};

inline DrawingKit_var _tmp(DrawingKitRef p) { return DrawingKit_var(p); }

class DrawingKitStub : public DrawingKit {
public:
    DrawingKitStub(Exchange*);
    ~DrawingKitStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfBrushStub_create(Exchange*);

class Brush : public FrescoObject {
protected:
    Brush();
    virtual ~Brush();
public:
    BrushRef _obj() { return this; };
    static BrushRef  _return_ref(Brush_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static BrushRef _narrow(BaseObjectRef);
    static BrushRef _duplicate(BrushRef obj) {
        return (BrushRef)_BaseObject__duplicate(obj, &_XfBrushStub_create);
    }
    virtual Coord width();
    virtual Boolean equal(Brush_in b);
    virtual ULong hash();
};

class Brush_var {
protected:
    BrushRef _obj_;
public:
    Brush_var() { _obj_ = 0; };
    Brush_var(BrushRef p) { _obj_ = p; }
    Brush_var(const Brush_var& r) {
        _obj_ = Brush::_duplicate(r._obj_);
    }
    ~Brush_var() { _BaseObject__release(_obj_); }

    Brush_var& operator =(BrushRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Brush::_duplicate(p);
        return *this;
    }
    Brush_var& operator =(const Brush_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Brush::_duplicate(r._obj_);
        return *this;
    }

    BrushRef _obj() const { return _obj_; }
    BrushRef& _out() { return _obj_; }
    operator BrushRef() const { return _obj_; }
    BrushRef operator ->() const { return _obj_; }
};

inline Brush_var _tmp(BrushRef p) { return Brush_var(p); }

class BrushStub : public Brush {
public:
    BrushStub(Exchange*);
    ~BrushStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfFontStub_create(Exchange*);

class Font : public FrescoObject {
protected:
    Font();
    virtual ~Font();
public:
    FontRef _obj() { return this; };
    static FontRef  _return_ref(Font_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static FontRef _narrow(BaseObjectRef);
    static FontRef _duplicate(FontRef obj) {
        return (FontRef)_BaseObject__duplicate(obj, &_XfFontStub_create);
    }
    struct Info {
        Coord width, height;
        Coord left_bearing, right_bearing;
        Coord ascent, descent;
        Coord font_ascent, font_descent;
    };
    virtual Boolean equal(Font_in f);
    virtual ULong hash();
    virtual CharString_return name();
    virtual CharString_return encoding();
    virtual Coord point_size();
    virtual void font_info(Font::Info& i);
    virtual void char_info(CharCode c, Font::Info& i);
    virtual void string_info(CharString_in s, Font::Info& i);
};

class Font_var {
protected:
    FontRef _obj_;
public:
    Font_var() { _obj_ = 0; };
    Font_var(FontRef p) { _obj_ = p; }
    Font_var(const Font_var& r) {
        _obj_ = Font::_duplicate(r._obj_);
    }
    ~Font_var() { _BaseObject__release(_obj_); }

    Font_var& operator =(FontRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Font::_duplicate(p);
        return *this;
    }
    Font_var& operator =(const Font_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Font::_duplicate(r._obj_);
        return *this;
    }

    FontRef _obj() const { return _obj_; }
    FontRef& _out() { return _obj_; }
    operator FontRef() const { return _obj_; }
    FontRef operator ->() const { return _obj_; }
};

inline Font_var _tmp(FontRef p) { return Font_var(p); }

class FontStub : public Font {
public:
    FontStub(Exchange*);
    ~FontStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfPatternStub_create(Exchange*);

class Pattern : public FrescoObject {
protected:
    Pattern();
    virtual ~Pattern();
public:
    PatternRef _obj() { return this; };
    static PatternRef  _return_ref(Pattern_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static PatternRef _narrow(BaseObjectRef);
    static PatternRef _duplicate(PatternRef obj) {
        return (PatternRef)_BaseObject__duplicate(obj, &_XfPatternStub_create);
    }
    virtual Boolean equal(Pattern_in p);
    virtual ULong hash();
    virtual void stipple(DrawingKit::Data32& d);
};

class Pattern_var {
protected:
    PatternRef _obj_;
public:
    Pattern_var() { _obj_ = 0; };
    Pattern_var(PatternRef p) { _obj_ = p; }
    Pattern_var(const Pattern_var& r) {
        _obj_ = Pattern::_duplicate(r._obj_);
    }
    ~Pattern_var() { _BaseObject__release(_obj_); }

    Pattern_var& operator =(PatternRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Pattern::_duplicate(p);
        return *this;
    }
    Pattern_var& operator =(const Pattern_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Pattern::_duplicate(r._obj_);
        return *this;
    }

    PatternRef _obj() const { return _obj_; }
    PatternRef& _out() { return _obj_; }
    operator PatternRef() const { return _obj_; }
    PatternRef operator ->() const { return _obj_; }
};

inline Pattern_var _tmp(PatternRef p) { return Pattern_var(p); }

class PatternStub : public Pattern {
public:
    PatternStub(Exchange*);
    ~PatternStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
