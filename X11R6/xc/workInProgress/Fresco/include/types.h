/* DO NOT EDIT -- Automatically generated from Interfaces/types.idl */

#ifndef Interfaces_types_h
#define Interfaces_types_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/Ox/object.h>

typedef ULong Tag;

class FrescoObject;
typedef FrescoObject* FrescoObjectRef;
typedef FrescoObjectRef FrescoObject_in;
typedef FrescoObjectRef FrescoObject_out, FrescoObject_inout;
typedef FrescoObjectRef FrescoObject_return;
class FrescoObject_var;

extern BaseObjectRef _XfFrescoObjectStub_create(Exchange*);

class FrescoObject : public BaseObject {
protected:
    FrescoObject();
    virtual ~FrescoObject();
public:
    FrescoObjectRef _obj() { return this; };
    static FrescoObjectRef  _return_ref(FrescoObject_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static FrescoObjectRef _narrow(BaseObjectRef);
    static FrescoObjectRef _duplicate(FrescoObjectRef obj) {
        return (FrescoObjectRef)_BaseObject__duplicate(obj, &_XfFrescoObjectStub_create);
    }
    virtual Long ref__(Long references);
    virtual Tag attach(FrescoObject_in observer);
    virtual void detach(Tag attach_tag);
    virtual void disconnect();
    virtual void notify_observers();
    virtual void update();
};

class FrescoObject_var {
protected:
    FrescoObjectRef _obj_;
public:
    FrescoObject_var() { _obj_ = 0; };
    FrescoObject_var(FrescoObjectRef p) { _obj_ = p; }
    FrescoObject_var(const FrescoObject_var& r) {
        _obj_ = FrescoObject::_duplicate(r._obj_);
    }
    ~FrescoObject_var() { _BaseObject__release(_obj_); }

    FrescoObject_var& operator =(FrescoObjectRef p) {
        _BaseObject__release(_obj_);
        _obj_ = FrescoObject::_duplicate(p);
        return *this;
    }
    FrescoObject_var& operator =(const FrescoObject_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = FrescoObject::_duplicate(r._obj_);
        return *this;
    }

    FrescoObjectRef _obj() const { return _obj_; }
    FrescoObjectRef& _out() { return _obj_; }
    operator FrescoObjectRef() const { return _obj_; }
    FrescoObjectRef operator ->() const { return _obj_; }
};

inline FrescoObject_var _tmp(FrescoObjectRef p) { return FrescoObject_var(p); }

class FrescoObjectStub : public FrescoObject {
public:
    FrescoObjectStub(Exchange*);
    ~FrescoObjectStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

typedef Long CharCode;

class CharString;
typedef CharString* CharStringRef;
typedef CharStringRef CharString_in;
typedef CharStringRef CharString_out, CharString_inout;
typedef CharStringRef CharString_return;
class CharString_var;

extern BaseObjectRef _XfCharStringStub_create(Exchange*);

class CharString : public FrescoObject {
protected:
    CharString();
    virtual ~CharString();
public:
    CharStringRef _obj() { return this; };
    static CharStringRef  _return_ref(CharString_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static CharStringRef _narrow(BaseObjectRef);
    static CharStringRef _duplicate(CharStringRef obj) {
        return (CharStringRef)_BaseObject__duplicate(obj, &_XfCharStringStub_create);
    }
    class Data {
    public:
        long _maximum, _length; CharCode* _buffer;

        Data() { _maximum = _length = 0; _buffer = 0; }
        Data(long m, long n, CharCode* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        Data(const Data& _s) { _buffer = 0; *this = _s; }
        Data& operator =(const Data&);
        ~Data() { delete [] _buffer; }
    };
    class CharData {
    public:
        long _maximum, _length; Char* _buffer;

        CharData() { _maximum = _length = 0; _buffer = 0; }
        CharData(long m, long n, Char* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        CharData(const CharData& _s) { _buffer = 0; *this = _s; }
        CharData& operator =(const CharData&);
        ~CharData() { delete [] _buffer; }
    };
    typedef ULong HashValue;
    virtual HashValue hash();
    virtual Long count();
    virtual void copy(CharString_in s);
    virtual Boolean equal(CharString_in s);
    virtual Boolean case_insensitive_equal(CharString_in s);
    virtual void get_data(Data& d);
    virtual void get_char_data(CharData& d);
    virtual void put_data(const Data& d);
    virtual void put_char_data(const CharData& d);
};

class CharString_var {
protected:
    CharStringRef _obj_;
public:
    CharString_var() { _obj_ = 0; };
    CharString_var(CharStringRef p) { _obj_ = p; }
    CharString_var(const CharString_var& r) {
        _obj_ = CharString::_duplicate(r._obj_);
    }
    ~CharString_var() { _BaseObject__release(_obj_); }

    CharString_var& operator =(CharStringRef p) {
        _BaseObject__release(_obj_);
        _obj_ = CharString::_duplicate(p);
        return *this;
    }
    CharString_var& operator =(const CharString_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = CharString::_duplicate(r._obj_);
        return *this;
    }

    CharStringRef _obj() const { return _obj_; }
    CharStringRef& _out() { return _obj_; }
    operator CharStringRef() const { return _obj_; }
    CharStringRef operator ->() const { return _obj_; }
};

inline CharString_var _tmp(CharStringRef p) { return CharString_var(p); }

class CharStringStub : public CharString {
public:
    CharStringStub(Exchange*);
    ~CharStringStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

typedef Float Coord;

typedef Long PixelCoord;

enum Axis {
    X_axis, Y_axis, Z_axis
};

struct Vertex {
    Coord x, y, z;
};

typedef Float Alignment;

class Action;
typedef Action* ActionRef;
typedef ActionRef Action_in;
typedef ActionRef Action_out, Action_inout;
typedef ActionRef Action_return;
class Action_var;

extern BaseObjectRef _XfActionStub_create(Exchange*);

class Action : public FrescoObject {
protected:
    Action();
    virtual ~Action();
public:
    ActionRef _obj() { return this; };
    static ActionRef  _return_ref(Action_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static ActionRef _narrow(BaseObjectRef);
    static ActionRef _duplicate(ActionRef obj) {
        return (ActionRef)_BaseObject__duplicate(obj, &_XfActionStub_create);
    }
    virtual void execute();
    virtual Boolean reversible();
    virtual void unexecute();
};

class Action_var {
protected:
    ActionRef _obj_;
public:
    Action_var() { _obj_ = 0; };
    Action_var(ActionRef p) { _obj_ = p; }
    Action_var(const Action_var& r) {
        _obj_ = Action::_duplicate(r._obj_);
    }
    ~Action_var() { _BaseObject__release(_obj_); }

    Action_var& operator =(ActionRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Action::_duplicate(p);
        return *this;
    }
    Action_var& operator =(const Action_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Action::_duplicate(r._obj_);
        return *this;
    }

    ActionRef _obj() const { return _obj_; }
    ActionRef& _out() { return _obj_; }
    operator ActionRef() const { return _obj_; }
    ActionRef operator ->() const { return _obj_; }
};

inline Action_var _tmp(ActionRef p) { return Action_var(p); }

class ActionStub : public Action {
public:
    ActionStub(Exchange*);
    ~ActionStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class Transform;
typedef Transform* TransformRef;
typedef TransformRef Transform_in;
typedef TransformRef Transform_out, Transform_inout;
typedef TransformRef Transform_return;
class Transform_var;

class Region;
typedef Region* RegionRef;
typedef RegionRef Region_in;
typedef RegionRef Region_out, Region_inout;
typedef RegionRef Region_return;
class Region_var;

extern BaseObjectRef _XfRegionStub_create(Exchange*);

class Region : public FrescoObject {
protected:
    Region();
    virtual ~Region();
public:
    RegionRef _obj() { return this; };
    static RegionRef  _return_ref(Region_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static RegionRef _narrow(BaseObjectRef);
    static RegionRef _duplicate(RegionRef obj) {
        return (RegionRef)_BaseObject__duplicate(obj, &_XfRegionStub_create);
    }
    struct BoundingSpan {
        Coord begin, end;
        Coord origin, length;
        Alignment align;
    };
    virtual Boolean contains(const Vertex& v);
    virtual Boolean contains_plane(const Vertex& v, Axis a);
    virtual Boolean intersects(Region_in r);
    virtual void copy(Region_in r);
    virtual void merge_intersect(Region_in r);
    virtual void merge_union(Region_in r);
    virtual void subtract(Region_in r);
    virtual void apply_transform(Transform_in t);
    virtual void bounds(Vertex& lower, Vertex& upper);
    virtual void origin(Vertex& v);
    virtual void span(Axis a, Region::BoundingSpan& s);
};

class Region_var {
protected:
    RegionRef _obj_;
public:
    Region_var() { _obj_ = 0; };
    Region_var(RegionRef p) { _obj_ = p; }
    Region_var(const Region_var& r) {
        _obj_ = Region::_duplicate(r._obj_);
    }
    ~Region_var() { _BaseObject__release(_obj_); }

    Region_var& operator =(RegionRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Region::_duplicate(p);
        return *this;
    }
    Region_var& operator =(const Region_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Region::_duplicate(r._obj_);
        return *this;
    }

    RegionRef _obj() const { return _obj_; }
    RegionRef& _out() { return _obj_; }
    operator RegionRef() const { return _obj_; }
    RegionRef operator ->() const { return _obj_; }
};

inline Region_var _tmp(RegionRef p) { return Region_var(p); }

class RegionStub : public Region {
public:
    RegionStub(Exchange*);
    ~RegionStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class StyleValue;
typedef StyleValue* StyleValueRef;
typedef StyleValueRef StyleValue_in;
typedef StyleValueRef StyleValue_out, StyleValue_inout;
typedef StyleValueRef StyleValue_return;
class StyleValue_var;

extern BaseObjectRef _XfStyleValueStub_create(Exchange*);

class StyleValue : public FrescoObject {
protected:
    StyleValue();
    virtual ~StyleValue();
public:
    StyleValueRef _obj() { return this; };
    static StyleValueRef  _return_ref(StyleValue_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static StyleValueRef _narrow(BaseObjectRef);
    static StyleValueRef _duplicate(StyleValueRef obj) {
        return (StyleValueRef)_BaseObject__duplicate(obj, &_XfStyleValueStub_create);
    }
    virtual CharString_return name();
    virtual Boolean uninitialized();
    virtual Long priority();
    virtual void priority(Long _p);
    virtual Boolean is_on();
    virtual Boolean read_boolean(Boolean& b);
    virtual void write_boolean(Boolean b);
    virtual Boolean read_coord(Coord& c);
    virtual void write_coord(Coord c);
    virtual Boolean read_integer(Long& i);
    virtual void write_integer(Long i);
    virtual Boolean read_real(Double& d);
    virtual void write_real(Double d);
    virtual Boolean read_string(CharString_out& s);
    virtual void write_string(CharString_in s);
    virtual Boolean read_value(FrescoObject_out& s);
    virtual void write_value(FrescoObject_in s);
    virtual void lock();
    virtual void unlock();
};

class StyleValue_var {
protected:
    StyleValueRef _obj_;
public:
    StyleValue_var() { _obj_ = 0; };
    StyleValue_var(StyleValueRef p) { _obj_ = p; }
    StyleValue_var(const StyleValue_var& r) {
        _obj_ = StyleValue::_duplicate(r._obj_);
    }
    ~StyleValue_var() { _BaseObject__release(_obj_); }

    StyleValue_var& operator =(StyleValueRef p) {
        _BaseObject__release(_obj_);
        _obj_ = StyleValue::_duplicate(p);
        return *this;
    }
    StyleValue_var& operator =(const StyleValue_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = StyleValue::_duplicate(r._obj_);
        return *this;
    }

    StyleValueRef _obj() const { return _obj_; }
    StyleValueRef& _out() { return _obj_; }
    operator StyleValueRef() const { return _obj_; }
    StyleValueRef operator ->() const { return _obj_; }
};

inline StyleValue_var _tmp(StyleValueRef p) { return StyleValue_var(p); }

class StyleValueStub : public StyleValue {
public:
    StyleValueStub(Exchange*);
    ~StyleValueStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class StyleVisitor;
typedef StyleVisitor* StyleVisitorRef;
typedef StyleVisitorRef StyleVisitor_in;
typedef StyleVisitorRef StyleVisitor_out, StyleVisitor_inout;
typedef StyleVisitorRef StyleVisitor_return;
class StyleVisitor_var;

class Style;
typedef Style* StyleRef;
typedef StyleRef Style_in;
typedef StyleRef Style_out, Style_inout;
typedef StyleRef Style_return;
class Style_var;

extern BaseObjectRef _XfStyleStub_create(Exchange*);

class Style : public FrescoObject {
protected:
    Style();
    virtual ~Style();
public:
    StyleRef _obj() { return this; };
    static StyleRef  _return_ref(Style_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static StyleRef _narrow(BaseObjectRef);
    static StyleRef _duplicate(StyleRef obj) {
        return (StyleRef)_BaseObject__duplicate(obj, &_XfStyleStub_create);
    }
    virtual Style_return new_style();
    virtual Style_return parent_style();
    virtual void link_parent(Style_in parent);
    virtual void unlink_parent();
    virtual Tag link_child(Style_in child);
    virtual void unlink_child(Tag link_tag);
    virtual void merge(Style_in s);
    virtual CharString_return name();
    virtual void name(CharString_in _p);
    virtual void alias(CharString_in s);
    virtual Boolean is_on(CharString_in name);
    virtual StyleValue_return bind(CharString_in name);
    virtual void unbind(CharString_in name);
    virtual StyleValue_return resolve(CharString_in name);
    virtual StyleValue_return resolve_wildcard(CharString_in name, Style_in start);
    virtual Long match(CharString_in name);
    virtual void visit_aliases(StyleVisitor_in v);
    virtual void visit_attributes(StyleVisitor_in v);
    virtual void visit_styles(StyleVisitor_in v);
    virtual void lock();
    virtual void unlock();
};

class Style_var {
protected:
    StyleRef _obj_;
public:
    Style_var() { _obj_ = 0; };
    Style_var(StyleRef p) { _obj_ = p; }
    Style_var(const Style_var& r) {
        _obj_ = Style::_duplicate(r._obj_);
    }
    ~Style_var() { _BaseObject__release(_obj_); }

    Style_var& operator =(StyleRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Style::_duplicate(p);
        return *this;
    }
    Style_var& operator =(const Style_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Style::_duplicate(r._obj_);
        return *this;
    }

    StyleRef _obj() const { return _obj_; }
    StyleRef& _out() { return _obj_; }
    operator StyleRef() const { return _obj_; }
    StyleRef operator ->() const { return _obj_; }
};

inline Style_var _tmp(StyleRef p) { return Style_var(p); }

class StyleStub : public Style {
public:
    StyleStub(Exchange*);
    ~StyleStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfStyleVisitorStub_create(Exchange*);

class StyleVisitor : public FrescoObject {
protected:
    StyleVisitor();
    virtual ~StyleVisitor();
public:
    StyleVisitorRef _obj() { return this; };
    static StyleVisitorRef  _return_ref(StyleVisitor_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static StyleVisitorRef _narrow(BaseObjectRef);
    static StyleVisitorRef _duplicate(StyleVisitorRef obj) {
        return (StyleVisitorRef)_BaseObject__duplicate(obj, &_XfStyleVisitorStub_create);
    }
    virtual Boolean visit_alias(CharString_in name);
    virtual Boolean visit_attribute(StyleValue_in a);
    virtual Boolean visit_style(Style_in s);
};

class StyleVisitor_var {
protected:
    StyleVisitorRef _obj_;
public:
    StyleVisitor_var() { _obj_ = 0; };
    StyleVisitor_var(StyleVisitorRef p) { _obj_ = p; }
    StyleVisitor_var(const StyleVisitor_var& r) {
        _obj_ = StyleVisitor::_duplicate(r._obj_);
    }
    ~StyleVisitor_var() { _BaseObject__release(_obj_); }

    StyleVisitor_var& operator =(StyleVisitorRef p) {
        _BaseObject__release(_obj_);
        _obj_ = StyleVisitor::_duplicate(p);
        return *this;
    }
    StyleVisitor_var& operator =(const StyleVisitor_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = StyleVisitor::_duplicate(r._obj_);
        return *this;
    }

    StyleVisitorRef _obj() const { return _obj_; }
    StyleVisitorRef& _out() { return _obj_; }
    operator StyleVisitorRef() const { return _obj_; }
    StyleVisitorRef operator ->() const { return _obj_; }
};

inline StyleVisitor_var _tmp(StyleVisitorRef p) { return StyleVisitor_var(p); }

class StyleVisitorStub : public StyleVisitor {
public:
    StyleVisitorStub(Exchange*);
    ~StyleVisitorStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfTransformStub_create(Exchange*);

class Transform : public FrescoObject {
protected:
    Transform();
    virtual ~Transform();
public:
    TransformRef _obj() { return this; };
    static TransformRef  _return_ref(Transform_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static TransformRef _narrow(BaseObjectRef);
    static TransformRef _duplicate(TransformRef obj) {
        return (TransformRef)_BaseObject__duplicate(obj, &_XfTransformStub_create);
    }
    typedef Coord Matrix[4][4];
    virtual void load(Transform_in t);
    virtual void load_identity();
    virtual void load_matrix(Matrix m);
    virtual void store_matrix(Matrix m);
    virtual Boolean equal(Transform_in t);
    virtual Boolean identity();
    virtual Boolean det_is_zero();
    virtual void scale(const Vertex& v);
    virtual void rotate(Float angle, Axis a);
    virtual void translate(const Vertex& v);
    virtual void premultiply(Transform_in t);
    virtual void postmultiply(Transform_in t);
    virtual void invert();
    virtual void transform_vertex(Vertex& v);
    virtual void inverse_transform_vertex(Vertex& v);
};

class Transform_var {
protected:
    TransformRef _obj_;
public:
    Transform_var() { _obj_ = 0; };
    Transform_var(TransformRef p) { _obj_ = p; }
    Transform_var(const Transform_var& r) {
        _obj_ = Transform::_duplicate(r._obj_);
    }
    ~Transform_var() { _BaseObject__release(_obj_); }

    Transform_var& operator =(TransformRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Transform::_duplicate(p);
        return *this;
    }
    Transform_var& operator =(const Transform_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Transform::_duplicate(r._obj_);
        return *this;
    }

    TransformRef _obj() const { return _obj_; }
    TransformRef& _out() { return _obj_; }
    operator TransformRef() const { return _obj_; }
    TransformRef operator ->() const { return _obj_; }
};

inline Transform_var _tmp(TransformRef p) { return Transform_var(p); }

class TransformStub : public Transform {
public:
    TransformStub(Exchange*);
    ~TransformStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
