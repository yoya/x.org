/* DO NOT EDIT -- Automatically generated from Interfaces/naming.idl */

#ifndef Interfaces_naming_h
#define Interfaces_naming_h

#include <X11/Fresco/Ox/base.h>

class NamingContext;
typedef NamingContext* NamingContextRef;
typedef NamingContextRef NamingContext_in;
typedef NamingContextRef NamingContext_out, NamingContext_inout;
typedef NamingContextRef NamingContext_return;
class NamingContext_var;

extern BaseObjectRef _NamingContextStub_create(Exchange*);

class NamingContext : public BaseObject {
protected:
    NamingContext();
    virtual ~NamingContext();
public:
    NamingContextRef _obj() { return this; };
    static NamingContextRef  _return_ref(NamingContext_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static NamingContextRef _narrow(BaseObjectRef);
    static NamingContextRef _duplicate(NamingContextRef obj) {
        return (NamingContextRef)_BaseObject__duplicate(obj, &_NamingContextStub_create);
    }
    typedef string NameComponent;
    class Name {
    public:
        long _maximum, _length; NameComponent* _buffer;

        Name() { _maximum = _length = 0; _buffer = 0; }
        Name(long m, long n, NameComponent* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        Name(const Name& _s) { _buffer = 0; *this = _s; }
        Name& operator =(const Name&);
        ~Name() { delete [] _buffer; }
    };
    enum BindingType {
        context_type, symbolic_link_type, ordered_merge_type, regular_type
    };
    struct BindingInfo {
        NameComponent comp;
        BindingType bt;
    };
    class BindingInfoList {
    public:
        long _maximum, _length; BindingInfo* _buffer;

        BindingInfoList() { _maximum = _length = 0; _buffer = 0; }
        BindingInfoList(long m, long n, BindingInfo* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        BindingInfoList(const BindingInfoList& _s) { _buffer = 0; *this = _s; }
        BindingInfoList& operator =(const BindingInfoList&);
        ~BindingInfoList() { delete [] _buffer; }
    };
    class NotFound : public UserException {
    public:
        enum { _index = 1, _code = 19984385 };
        NotFound();
        NotFound(Long mode);
        static NotFound* _cast(const Exception*);

        void _put(MarshalBuffer&) const;
        static Exception* _get(MarshalBuffer&);

        Long mode;
    };
    class NotContext : public UserException {
    public:
        enum { _index = 2, _code = 19984386 };
        NotContext();
        static NotContext* _cast(const Exception*);

        void _put(MarshalBuffer&) const;
        static Exception* _get(MarshalBuffer&);
    };
    class PermissionDenied : public UserException {
    public:
        enum { _index = 3, _code = 19984387 };
        PermissionDenied();
        static PermissionDenied* _cast(const Exception*);

        void _put(MarshalBuffer&) const;
        static Exception* _get(MarshalBuffer&);
    };
    virtual BaseObject_return resolve(const Name& n, Env* _env = 0);
    virtual BindingInfoList list(const Name& n, Env* _env = 0);
};

class NamingContext_var {
protected:
    NamingContextRef _obj_;
public:
    NamingContext_var() { _obj_ = 0; };
    NamingContext_var(NamingContextRef p) { _obj_ = p; }
    NamingContext_var(const NamingContext_var& r) {
        _obj_ = NamingContext::_duplicate(r._obj_);
    }
    ~NamingContext_var() { _BaseObject__release(_obj_); }

    NamingContext_var& operator =(NamingContextRef p) {
        _BaseObject__release(_obj_);
        _obj_ = NamingContext::_duplicate(p);
        return *this;
    }
    NamingContext_var& operator =(const NamingContext_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = NamingContext::_duplicate(r._obj_);
        return *this;
    }

    NamingContextRef _obj() const { return _obj_; }
    NamingContextRef& _out() { return _obj_; }
    operator NamingContextRef() const { return _obj_; }
    NamingContextRef operator ->() const { return _obj_; }
};

inline NamingContext_var _tmp(NamingContextRef p) { return NamingContext_var(p); }

class NamingContextStub : public NamingContext {
public:
    NamingContextStub(Exchange*);
    ~NamingContextStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
