/* DO NOT EDIT -- Automatically generated from Interfaces/figures.idl */

#ifndef Interfaces_figures_h
#define Interfaces_figures_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/glyph.h>

class FigureStyle;
typedef FigureStyle* FigureStyleRef;
typedef FigureStyleRef FigureStyle_in;
typedef FigureStyleRef FigureStyle_out, FigureStyle_inout;
typedef FigureStyleRef FigureStyle_return;
class FigureStyle_var;

extern BaseObjectRef _XfFigureStyleStub_create(Exchange*);

class FigureStyle : public Style {
protected:
    FigureStyle();
    virtual ~FigureStyle();
public:
    FigureStyleRef _obj() { return this; };
    static FigureStyleRef  _return_ref(FigureStyle_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static FigureStyleRef _narrow(BaseObjectRef);
    static FigureStyleRef _duplicate(FigureStyleRef obj) {
        return (FigureStyleRef)_BaseObject__duplicate(obj, &_XfFigureStyleStub_create);
    }
    virtual Color_return background();
    virtual void background(Color_in _p);
    virtual Brush_return brush_attr();
    virtual void brush_attr(Brush_in _p);
    virtual Font_return font_attr();
    virtual void font_attr(Font_in _p);
    virtual Color_return foreground();
    virtual void foreground(Color_in _p);
};

class FigureStyle_var {
protected:
    FigureStyleRef _obj_;
public:
    FigureStyle_var() { _obj_ = 0; };
    FigureStyle_var(FigureStyleRef p) { _obj_ = p; }
    FigureStyle_var(const FigureStyle_var& r) {
        _obj_ = FigureStyle::_duplicate(r._obj_);
    }
    ~FigureStyle_var() { _BaseObject__release(_obj_); }

    FigureStyle_var& operator =(FigureStyleRef p) {
        _BaseObject__release(_obj_);
        _obj_ = FigureStyle::_duplicate(p);
        return *this;
    }
    FigureStyle_var& operator =(const FigureStyle_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = FigureStyle::_duplicate(r._obj_);
        return *this;
    }

    FigureStyleRef _obj() const { return _obj_; }
    FigureStyleRef& _out() { return _obj_; }
    operator FigureStyleRef() const { return _obj_; }
    FigureStyleRef operator ->() const { return _obj_; }
};

inline FigureStyle_var _tmp(FigureStyleRef p) { return FigureStyle_var(p); }

class FigureStyleStub : public FigureStyle {
public:
    FigureStyleStub(Exchange*);
    ~FigureStyleStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class FigureKit;
typedef FigureKit* FigureKitRef;
typedef FigureKitRef FigureKit_in;
typedef FigureKitRef FigureKit_out, FigureKit_inout;
typedef FigureKitRef FigureKit_return;
class FigureKit_var;

extern BaseObjectRef _XfFigureKitStub_create(Exchange*);

class FigureKit : public FrescoObject {
protected:
    FigureKit();
    virtual ~FigureKit();
public:
    FigureKitRef _obj() { return this; };
    static FigureKitRef  _return_ref(FigureKit_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static FigureKitRef _narrow(BaseObjectRef);
    static FigureKitRef _duplicate(FigureKitRef obj) {
        return (FigureKitRef)_BaseObject__duplicate(obj, &_XfFigureKitStub_create);
    }
    class Vertices {
    public:
        long _maximum, _length; Vertex* _buffer;

        Vertices() { _maximum = _length = 0; _buffer = 0; }
        Vertices(long m, long n, Vertex* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        Vertices(const Vertices& _s) { _buffer = 0; *this = _s; }
        Vertices& operator =(const Vertices&);
        ~Vertices() { delete [] _buffer; }
    };
    enum Mode {
        fill, stroke, fill_stroke
    };
    virtual FigureStyle_return default_style();
    virtual FigureStyle_return new_style(Style_in parent);
    virtual Glyph_return figure_root(Glyph_in child);
    virtual Glyph_return label(FigureStyle_in s, CharString_in str);
    virtual Glyph_return point(FigureStyle_in s, Coord x, Coord y);
    virtual Glyph_return line(FigureStyle_in s, Coord x0, Coord y0, Coord x1, Coord y1);
    virtual Glyph_return rectangle(FigureKit::Mode m, FigureStyle_in s, Coord left, Coord bottom, Coord right, Coord top);
    virtual Glyph_return circle(FigureKit::Mode m, FigureStyle_in s, Coord x, Coord y, Coord r);
    virtual Glyph_return ellipse(FigureKit::Mode m, FigureStyle_in s, Coord x, Coord y, Coord r1, Coord r2);
    virtual Glyph_return open_bspline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v);
    virtual Glyph_return closed_bspline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v);
    virtual Glyph_return multiline(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v);
    virtual Glyph_return polygon(FigureKit::Mode m, FigureStyle_in s, const FigureKit::Vertices& v);
    virtual Glyph_return fitter(Glyph_in g);
    virtual Glyph_return group();
};

class FigureKit_var {
protected:
    FigureKitRef _obj_;
public:
    FigureKit_var() { _obj_ = 0; };
    FigureKit_var(FigureKitRef p) { _obj_ = p; }
    FigureKit_var(const FigureKit_var& r) {
        _obj_ = FigureKit::_duplicate(r._obj_);
    }
    ~FigureKit_var() { _BaseObject__release(_obj_); }

    FigureKit_var& operator =(FigureKitRef p) {
        _BaseObject__release(_obj_);
        _obj_ = FigureKit::_duplicate(p);
        return *this;
    }
    FigureKit_var& operator =(const FigureKit_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = FigureKit::_duplicate(r._obj_);
        return *this;
    }

    FigureKitRef _obj() const { return _obj_; }
    FigureKitRef& _out() { return _obj_; }
    operator FigureKitRef() const { return _obj_; }
    FigureKitRef operator ->() const { return _obj_; }
};

inline FigureKit_var _tmp(FigureKitRef p) { return FigureKit_var(p); }

class FigureKitStub : public FigureKit {
public:
    FigureKitStub(Exchange*);
    ~FigureKitStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
