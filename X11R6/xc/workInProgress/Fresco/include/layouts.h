/* DO NOT EDIT -- Automatically generated from Interfaces/layouts.idl */

#ifndef Interfaces_layouts_h
#define Interfaces_layouts_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/glyph.h>

class Color;
typedef Color* ColorRef;
typedef ColorRef Color_in;
typedef ColorRef Color_out, Color_inout;
typedef ColorRef Color_return;
class Color_var;

class Font;
typedef Font* FontRef;
typedef FontRef Font_in;
typedef FontRef Font_out, Font_inout;
typedef FontRef Font_return;
class Font_var;

class LayoutKit;
typedef LayoutKit* LayoutKitRef;
typedef LayoutKitRef LayoutKit_in;
typedef LayoutKitRef LayoutKit_out, LayoutKit_inout;
typedef LayoutKitRef LayoutKit_return;
class LayoutKit_var;

extern BaseObjectRef _XfLayoutKitStub_create(Exchange*);

class LayoutKit : public FrescoObject {
protected:
    LayoutKit();
    virtual ~LayoutKit();
public:
    LayoutKitRef _obj() { return this; };
    static LayoutKitRef  _return_ref(LayoutKit_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static LayoutKitRef _narrow(BaseObjectRef);
    static LayoutKitRef _duplicate(LayoutKitRef obj) {
        return (LayoutKitRef)_BaseObject__duplicate(obj, &_XfLayoutKitStub_create);
    }
    virtual Coord fil();
    virtual void fil(Coord _p);
    virtual Glyph_return hbox();
    virtual Glyph_return vbox();
    virtual Glyph_return hbox_first_aligned();
    virtual Glyph_return vbox_first_aligned();
    virtual Glyph_return overlay();
    virtual Glyph_return deck();
    virtual Glyph_return back(Glyph_in g, Glyph_in under);
    virtual Glyph_return front(Glyph_in g, Glyph_in over);
    virtual Glyph_return between(Glyph_in g, Glyph_in under, Glyph_in over);
    virtual Glyph_return glue(Axis a, Coord natural, Coord stretch, Coord shrink, Alignment align);
    virtual Glyph_return glue_requisition(const Glyph::Requisition& r);
    virtual Glyph_return hfil();
    virtual Glyph_return hglue_fil(Coord natural);
    virtual Glyph_return hglue(Coord natural, Coord stretch, Coord shrink);
    virtual Glyph_return hglue_aligned(Coord natural, Coord stretch, Coord shrink, Alignment a);
    virtual Glyph_return hspace(Coord natural);
    virtual Glyph_return vfil();
    virtual Glyph_return vglue_fil(Coord natural);
    virtual Glyph_return vglue(Coord natural, Coord stretch, Coord shrink);
    virtual Glyph_return vglue_aligned(Coord natural, Coord stretch, Coord shrink, Alignment a);
    virtual Glyph_return vspace(Coord natural);
    virtual Glyph_return shape_of(Glyph_in g);
    virtual Glyph_return shape_of_xy(Glyph_in gx, Glyph_in gy);
    virtual Glyph_return shape_of_xyz(Glyph_in gx, Glyph_in gy, Glyph_in gz);
    virtual Glyph_return strut(Font_in f, Coord natural, Coord stretch, Coord shrink);
    virtual Glyph_return hstrut(Coord right_bearing, Coord left_bearing, Coord natural, Coord stretch, Coord shrink);
    virtual Glyph_return vstrut(Coord ascent, Coord descent, Coord natural, Coord stretch, Coord shrink);
    virtual Glyph_return spaces(Long count, Coord each, Font_in f, Color_in c);
    virtual Glyph_return center(Glyph_in g);
    virtual Glyph_return center_aligned(Glyph_in g, Alignment x, Alignment y);
    virtual Glyph_return center_axis(Glyph_in g, Axis a, Alignment align);
    virtual Glyph_return hcenter(Glyph_in g);
    virtual Glyph_return hcenter_aligned(Glyph_in g, Alignment x);
    virtual Glyph_return vcenter(Glyph_in g);
    virtual Glyph_return vcenter_aligned(Glyph_in g, Alignment y);
    virtual Glyph_return fixed(Glyph_in g, Coord x, Coord y);
    virtual Glyph_return fixed_axis(Glyph_in g, Axis a, Coord size);
    virtual Glyph_return hfixed(Glyph_in g, Coord x);
    virtual Glyph_return vfixed(Glyph_in g, Coord y);
    virtual Glyph_return flexible(Glyph_in g, Coord stretch, Coord shrink);
    virtual Glyph_return flexible_fil(Glyph_in g);
    virtual Glyph_return flexible_axis(Glyph_in g, Axis a, Coord stretch, Coord shrink);
    virtual Glyph_return hflexible(Glyph_in g, Coord stretch, Coord shrink);
    virtual Glyph_return vflexible(Glyph_in g, Coord stretch, Coord shrink);
    virtual Glyph_return natural(Glyph_in g, Coord x, Coord y);
    virtual Glyph_return natural_axis(Glyph_in g, Axis a, Coord size);
    virtual Glyph_return hnatural(Glyph_in g, Coord x);
    virtual Glyph_return vnatural(Glyph_in g, Coord y);
    virtual Glyph_return margin(Glyph_in g, Coord all);
    virtual Glyph_return margin_lrbt(Glyph_in g, Coord lmargin, Coord rmargin, Coord bmargin, Coord tmargin);
    virtual Glyph_return margin_lrbt_flexible(Glyph_in g, Coord lmargin, Coord lstretch, Coord lshrink, Coord rmargin, Coord rstretch, Coord rshrink, Coord bmargin, Coord bstretch, Coord bshrink, Coord tmargin, Coord tstretch, Coord tshrink);
    virtual Glyph_return hmargin(Glyph_in g, Coord both);
    virtual Glyph_return hmargin_lr(Glyph_in g, Coord lmargin, Coord rmargin);
    virtual Glyph_return hmargin_lr_flexible(Glyph_in g, Coord lmargin, Coord lstretch, Coord lshrink, Coord rmargin, Coord rstretch, Coord rshrink);
    virtual Glyph_return vmargin(Glyph_in g, Coord both);
    virtual Glyph_return vmargin_bt(Glyph_in g, Coord bmargin, Coord tmargin);
    virtual Glyph_return vmargin_bt_flexible(Glyph_in g, Coord bmargin, Coord bstretch, Coord bshrink, Coord tmargin, Coord tstretch, Coord tshrink);
    virtual Glyph_return lmargin(Glyph_in g, Coord natural);
    virtual Glyph_return lmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink);
    virtual Glyph_return rmargin(Glyph_in g, Coord natural);
    virtual Glyph_return rmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink);
    virtual Glyph_return bmargin(Glyph_in g, Coord natural);
    virtual Glyph_return bmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink);
    virtual Glyph_return tmargin(Glyph_in g, Coord natural);
    virtual Glyph_return tmargin_flexible(Glyph_in g, Coord natural, Coord stretch, Coord shrink);
};

class LayoutKit_var {
protected:
    LayoutKitRef _obj_;
public:
    LayoutKit_var() { _obj_ = 0; };
    LayoutKit_var(LayoutKitRef p) { _obj_ = p; }
    LayoutKit_var(const LayoutKit_var& r) {
        _obj_ = LayoutKit::_duplicate(r._obj_);
    }
    ~LayoutKit_var() { _BaseObject__release(_obj_); }

    LayoutKit_var& operator =(LayoutKitRef p) {
        _BaseObject__release(_obj_);
        _obj_ = LayoutKit::_duplicate(p);
        return *this;
    }
    LayoutKit_var& operator =(const LayoutKit_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = LayoutKit::_duplicate(r._obj_);
        return *this;
    }

    LayoutKitRef _obj() const { return _obj_; }
    LayoutKitRef& _out() { return _obj_; }
    operator LayoutKitRef() const { return _obj_; }
    LayoutKitRef operator ->() const { return _obj_; }
};

inline LayoutKit_var _tmp(LayoutKitRef p) { return LayoutKit_var(p); }

class LayoutKitStub : public LayoutKit {
public:
    LayoutKitStub(Exchange*);
    ~LayoutKitStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
