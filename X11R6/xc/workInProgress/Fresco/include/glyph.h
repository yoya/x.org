/* DO NOT EDIT -- Automatically generated from Interfaces/glyph.idl */

#ifndef Interfaces_glyph_h
#define Interfaces_glyph_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>

class Damage;
typedef Damage* DamageRef;
typedef DamageRef Damage_in;
typedef DamageRef Damage_out, Damage_inout;
typedef DamageRef Damage_return;
class Damage_var;

extern BaseObjectRef _XfDamageStub_create(Exchange*);

class Damage : public FrescoObject {
protected:
    Damage();
    virtual ~Damage();
public:
    DamageRef _obj() { return this; };
    static DamageRef  _return_ref(Damage_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static DamageRef _narrow(BaseObjectRef);
    static DamageRef _duplicate(DamageRef obj) {
        return (DamageRef)_BaseObject__duplicate(obj, &_XfDamageStub_create);
    }
    virtual void incur();
    virtual void extend(Region_in r);
    virtual Region_return current();
};

class Damage_var {
protected:
    DamageRef _obj_;
public:
    Damage_var() { _obj_ = 0; };
    Damage_var(DamageRef p) { _obj_ = p; }
    Damage_var(const Damage_var& r) {
        _obj_ = Damage::_duplicate(r._obj_);
    }
    ~Damage_var() { _BaseObject__release(_obj_); }

    Damage_var& operator =(DamageRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Damage::_duplicate(p);
        return *this;
    }
    Damage_var& operator =(const Damage_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Damage::_duplicate(r._obj_);
        return *this;
    }

    DamageRef _obj() const { return _obj_; }
    DamageRef& _out() { return _obj_; }
    operator DamageRef() const { return _obj_; }
    DamageRef operator ->() const { return _obj_; }
};

inline Damage_var _tmp(DamageRef p) { return Damage_var(p); }

class DamageStub : public Damage {
public:
    DamageStub(Exchange*);
    ~DamageStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class GlyphOffset;
typedef GlyphOffset* GlyphOffsetRef;
typedef GlyphOffsetRef GlyphOffset_in;
typedef GlyphOffsetRef GlyphOffset_out, GlyphOffset_inout;
typedef GlyphOffsetRef GlyphOffset_return;
class GlyphOffset_var;

class GlyphTraversal;
typedef GlyphTraversal* GlyphTraversalRef;
typedef GlyphTraversalRef GlyphTraversal_in;
typedef GlyphTraversalRef GlyphTraversal_out, GlyphTraversal_inout;
typedef GlyphTraversalRef GlyphTraversal_return;
class GlyphTraversal_var;

class Glyph;
typedef Glyph* GlyphRef;
typedef GlyphRef Glyph_in;
typedef GlyphRef Glyph_out, Glyph_inout;
typedef GlyphRef Glyph_return;
class Glyph_var;

extern BaseObjectRef _XfGlyphStub_create(Exchange*);

class Glyph : public FrescoObject {
protected:
    Glyph();
    virtual ~Glyph();
public:
    GlyphRef _obj() { return this; };
    static GlyphRef  _return_ref(Glyph_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static GlyphRef _narrow(BaseObjectRef);
    static GlyphRef _duplicate(GlyphRef obj) {
        return (GlyphRef)_BaseObject__duplicate(obj, &_XfGlyphStub_create);
    }
    struct Requirement {
        Boolean defined;
        Coord natural, maximum, minimum;
        Alignment align;
    };
    struct Requisition {
        Requirement x, y, z;
        Boolean preserve_aspect;
    };
    struct AllocationInfo {
        RegionRef allocation;
        TransformRef transformation;
        DamageRef damaged;
    };
    class AllocationInfoSeq {
    public:
        long _maximum, _length; AllocationInfo* _buffer;

        AllocationInfoSeq() { _maximum = _length = 0; _buffer = 0; }
        AllocationInfoSeq(long m, long n, AllocationInfo* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        AllocationInfoSeq(const AllocationInfoSeq& _s) { _buffer = 0; *this = _s; }
        AllocationInfoSeq& operator =(const AllocationInfoSeq&);
        ~AllocationInfoSeq() { delete [] _buffer; }
    };
    class OffsetSeq {
    public:
        long _maximum, _length; GlyphOffsetRef* _buffer;

        OffsetSeq() { _maximum = _length = 0; _buffer = 0; }
        OffsetSeq(long m, long n, GlyphOffsetRef* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        OffsetSeq(const OffsetSeq& _s) { _buffer = 0; *this = _s; }
        OffsetSeq& operator =(const OffsetSeq&);
        ~OffsetSeq() { delete [] _buffer; }
    };
    virtual Glyph_return clone_glyph();
    virtual Style_return glyph_style();
    virtual void glyph_style(Style_in _p);
    virtual Transform_return transformation();
    virtual void request(Glyph::Requisition& r);
    virtual void extension(const Glyph::AllocationInfo& a, Region_in r);
    virtual void shape(Region_in r);
    virtual void traverse(GlyphTraversal_in t);
    virtual void draw(GlyphTraversal_in t);
    virtual void pick(GlyphTraversal_in t);
    virtual Glyph_return body();
    virtual void body(Glyph_in _p);
    virtual void append(Glyph_in g);
    virtual void prepend(Glyph_in g);
    virtual Tag add_parent(GlyphOffset_in parent_offset);
    virtual void remove_parent(Tag add_tag);
    virtual GlyphOffset_return first_child_offset();
    virtual GlyphOffset_return last_child_offset();
    virtual void parent_offsets(OffsetSeq& parents);
    virtual void allocations(Glyph::AllocationInfoSeq& a);
    virtual void need_redraw();
    virtual void need_redraw_region(Region_in r);
    virtual void need_resize();
    virtual Boolean restore_trail(GlyphTraversal_in t);
};

class Glyph_var {
protected:
    GlyphRef _obj_;
public:
    Glyph_var() { _obj_ = 0; };
    Glyph_var(GlyphRef p) { _obj_ = p; }
    Glyph_var(const Glyph_var& r) {
        _obj_ = Glyph::_duplicate(r._obj_);
    }
    ~Glyph_var() { _BaseObject__release(_obj_); }

    Glyph_var& operator =(GlyphRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Glyph::_duplicate(p);
        return *this;
    }
    Glyph_var& operator =(const Glyph_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Glyph::_duplicate(r._obj_);
        return *this;
    }

    GlyphRef _obj() const { return _obj_; }
    GlyphRef& _out() { return _obj_; }
    operator GlyphRef() const { return _obj_; }
    GlyphRef operator ->() const { return _obj_; }
};

inline Glyph_var _tmp(GlyphRef p) { return Glyph_var(p); }

class GlyphStub : public Glyph {
public:
    GlyphStub(Exchange*);
    ~GlyphStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfGlyphOffsetStub_create(Exchange*);

class GlyphOffset : public FrescoObject {
protected:
    GlyphOffset();
    virtual ~GlyphOffset();
public:
    GlyphOffsetRef _obj() { return this; };
    static GlyphOffsetRef  _return_ref(GlyphOffset_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static GlyphOffsetRef _narrow(BaseObjectRef);
    static GlyphOffsetRef _duplicate(GlyphOffsetRef obj) {
        return (GlyphOffsetRef)_BaseObject__duplicate(obj, &_XfGlyphOffsetStub_create);
    }
    virtual Glyph_return parent();
    virtual Glyph_return child();
    virtual GlyphOffset_return next_child();
    virtual GlyphOffset_return prev_child();
    virtual void allocations(Glyph::AllocationInfoSeq& a);
    virtual void insert(Glyph_in g);
    virtual void replace(Glyph_in g);
    virtual void remove();
    virtual void notify();
    virtual void visit_trail(GlyphTraversal_in t);
    virtual void child_allocate(Glyph::AllocationInfo& a);
};

class GlyphOffset_var {
protected:
    GlyphOffsetRef _obj_;
public:
    GlyphOffset_var() { _obj_ = 0; };
    GlyphOffset_var(GlyphOffsetRef p) { _obj_ = p; }
    GlyphOffset_var(const GlyphOffset_var& r) {
        _obj_ = GlyphOffset::_duplicate(r._obj_);
    }
    ~GlyphOffset_var() { _BaseObject__release(_obj_); }

    GlyphOffset_var& operator =(GlyphOffsetRef p) {
        _BaseObject__release(_obj_);
        _obj_ = GlyphOffset::_duplicate(p);
        return *this;
    }
    GlyphOffset_var& operator =(const GlyphOffset_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = GlyphOffset::_duplicate(r._obj_);
        return *this;
    }

    GlyphOffsetRef _obj() const { return _obj_; }
    GlyphOffsetRef& _out() { return _obj_; }
    operator GlyphOffsetRef() const { return _obj_; }
    GlyphOffsetRef operator ->() const { return _obj_; }
};

inline GlyphOffset_var _tmp(GlyphOffsetRef p) { return GlyphOffset_var(p); }

class GlyphOffsetStub : public GlyphOffset {
public:
    GlyphOffsetStub(Exchange*);
    ~GlyphOffsetStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class Viewer;
typedef Viewer* ViewerRef;
typedef ViewerRef Viewer_in;
typedef ViewerRef Viewer_out, Viewer_inout;
typedef ViewerRef Viewer_return;
class Viewer_var;

extern BaseObjectRef _XfGlyphTraversalStub_create(Exchange*);

class GlyphTraversal : public FrescoObject {
protected:
    GlyphTraversal();
    virtual ~GlyphTraversal();
public:
    GlyphTraversalRef _obj() { return this; };
    static GlyphTraversalRef  _return_ref(GlyphTraversal_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static GlyphTraversalRef _narrow(BaseObjectRef);
    static GlyphTraversalRef _duplicate(GlyphTraversalRef obj) {
        return (GlyphTraversalRef)_BaseObject__duplicate(obj, &_XfGlyphTraversalStub_create);
    }
    enum Operation {
        draw, pick_top, pick_any, pick_all, other
    };
    virtual Operation op();
    virtual Operation swap_op(Operation op);
    virtual void begin_viewer(Viewer_in v);
    virtual void end_viewer();
    virtual void traverse_child(GlyphOffset_in o, Region_in allocation);
    virtual void visit();
    virtual GlyphTraversal_return trail();
    virtual Glyph_return current_glyph();
    virtual GlyphOffset_return current_offset();
    virtual Viewer_return current_viewer();
    virtual Boolean forward();
    virtual Boolean backward();
    virtual Painter_return current_painter();
    virtual void current_painter(Painter_in _p);
    virtual Display_return current_display();
    virtual Screen_return current_screen();
    virtual Region_return allocation();
    virtual Boolean bounds(Vertex& lower, Vertex& upper, Vertex& origin);
    virtual Boolean allocation_is_visible();
    virtual Transform_return current_transform();
    virtual Damage_return damaged();
    virtual void hit();
    virtual Long hit_info();
    virtual void hit_info(Long _p);
    virtual GlyphTraversal_return picked();
    virtual void clear();
};

class GlyphTraversal_var {
protected:
    GlyphTraversalRef _obj_;
public:
    GlyphTraversal_var() { _obj_ = 0; };
    GlyphTraversal_var(GlyphTraversalRef p) { _obj_ = p; }
    GlyphTraversal_var(const GlyphTraversal_var& r) {
        _obj_ = GlyphTraversal::_duplicate(r._obj_);
    }
    ~GlyphTraversal_var() { _BaseObject__release(_obj_); }

    GlyphTraversal_var& operator =(GlyphTraversalRef p) {
        _BaseObject__release(_obj_);
        _obj_ = GlyphTraversal::_duplicate(p);
        return *this;
    }
    GlyphTraversal_var& operator =(const GlyphTraversal_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = GlyphTraversal::_duplicate(r._obj_);
        return *this;
    }

    GlyphTraversalRef _obj() const { return _obj_; }
    GlyphTraversalRef& _out() { return _obj_; }
    operator GlyphTraversalRef() const { return _obj_; }
    GlyphTraversalRef operator ->() const { return _obj_; }
};

inline GlyphTraversal_var _tmp(GlyphTraversalRef p) { return GlyphTraversal_var(p); }

class GlyphTraversalStub : public GlyphTraversal {
public:
    GlyphTraversalStub(Exchange*);
    ~GlyphTraversalStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
