/* DO NOT EDIT -- Automatically generated from Interfaces/object.idl */

#ifndef Interfaces_object_h
#define Interfaces_object_h

#include <X11/Fresco/Ox/base.h>

class TypeObj;
typedef TypeObj* TypeObjRef;
typedef TypeObjRef TypeObj_in;
typedef TypeObjRef TypeObj_out, TypeObj_inout;
typedef TypeObjRef TypeObj_return;
class TypeObj_var;

class TypeObj : public BaseObject {
protected:
    TypeObj();
    virtual ~TypeObj();
public:
    TypeObjRef _obj() { return this; };
    static TypeObjRef  _return_ref(TypeObj_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static TypeObjRef _narrow(BaseObjectRef);
    static TypeObjRef _duplicate(TypeObjRef obj) {
        return (TypeObjRef)_BaseObject__duplicate(obj, 0);
    }
    enum ParamMode {
        param_in, param_out, param_inout
    };
    struct ParamInfo {
        string param_name;
        ParamMode param_mode;
        TypeObjRef param_type;
    };
    struct OpInfo {
        string op_name;
        Long op_index;
        TypeObjRef op_result;
        class _ParamInfoSeq {
        public:
            long _maximum, _length; ParamInfo* _buffer;

            _ParamInfoSeq() { _maximum = _length = 0; _buffer = 0; }
            _ParamInfoSeq(long m, long n, ParamInfo* e) {
                _maximum = m; _length = n; _buffer = e;
            }
            _ParamInfoSeq(const _ParamInfoSeq& _s) { _buffer = 0; *this = _s; }
            _ParamInfoSeq& operator =(const _ParamInfoSeq&);
            ~_ParamInfoSeq() { delete [] _buffer; }
        } op_params;
    };
    enum KindOf {
        void_type, boolean_type, char_type, octet_type, short_type, 
        unsigned_short_type, long_type, unsigned_long_type, longlong_type, 
        unsigned_longlong_type, float_type, double_type, string_type, 
        enum_type, array_type, struct_type, sequence_type, interface_type, 
        typedef_type
    };
    virtual string name() = 0;
    virtual Boolean op_info(TypeObj::OpInfo& op, Long n) = 0;
    virtual KindOf kind() = 0;
    virtual Long enum_info() = 0;
    virtual void array_info(TypeObj_out& type, Long& size) = 0;
    virtual Long members() = 0;
    virtual TypeObj_return member_info(Long n) = 0;
    virtual void sequence_info(TypeObj_out& type, Long& size) = 0;
    virtual TypeObj_return typedef_info() = 0;
};

class TypeObj_var {
protected:
    TypeObjRef _obj_;
public:
    TypeObj_var() { _obj_ = 0; };
    TypeObj_var(TypeObjRef p) { _obj_ = p; }
    TypeObj_var(const TypeObj_var& r) {
        _obj_ = TypeObj::_duplicate(r._obj_);
    }
    ~TypeObj_var() { _BaseObject__release(_obj_); }

    TypeObj_var& operator =(TypeObjRef p) {
        _BaseObject__release(_obj_);
        _obj_ = TypeObj::_duplicate(p);
        return *this;
    }
    TypeObj_var& operator =(const TypeObj_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = TypeObj::_duplicate(r._obj_);
        return *this;
    }

    TypeObjRef _obj() const { return _obj_; }
    TypeObjRef& _out() { return _obj_; }
    operator TypeObjRef() const { return _obj_; }
    TypeObjRef operator ->() const { return _obj_; }
};

inline TypeObj_var _tmp(TypeObjRef p) { return TypeObj_var(p); }

class RequestObj;
typedef RequestObj* RequestObjRef;
typedef RequestObjRef RequestObj_in;
typedef RequestObjRef RequestObj_out, RequestObj_inout;
typedef RequestObjRef RequestObj_return;
class RequestObj_var;

class RequestObj : public BaseObject {
protected:
    RequestObj();
    virtual ~RequestObj();
public:
    RequestObjRef _obj() { return this; };
    static RequestObjRef  _return_ref(RequestObj_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static RequestObjRef _narrow(BaseObjectRef);
    static RequestObjRef _duplicate(RequestObjRef obj) {
        return (RequestObjRef)_BaseObject__duplicate(obj, 0);
    }
    enum CallStatus {
        initial, ok, unknown_operation, ambiguous_operation, bad_argument_count, 
        bad_argument_type, invoke_failed
    };
    virtual void set_operation(string s) = 0;
    virtual CallStatus invoke() = 0;
    virtual CallStatus op_info(TypeObj::OpInfo& op) = 0;
    virtual void put_char(Char value) = 0;
    virtual Char get_char() = 0;
    virtual void put_boolean(Boolean value) = 0;
    virtual Boolean get_boolean() = 0;
    virtual void put_octet(Octet value) = 0;
    virtual Octet get_octet() = 0;
    virtual void put_short(Short value) = 0;
    virtual Short get_short() = 0;
    virtual void put_unsigned_short(UShort value) = 0;
    virtual UShort get_unsigned_short() = 0;
    virtual void put_long(Long value) = 0;
    virtual Long get_long() = 0;
    virtual void put_unsigned_long(ULong value) = 0;
    virtual ULong get_unsigned_long() = 0;
    virtual void put_float(Float value) = 0;
    virtual Float get_float() = 0;
    virtual void put_double(Double value) = 0;
    virtual Double get_double() = 0;
    virtual void put_string(string value) = 0;
    virtual string get_string() = 0;
    virtual void put_object(BaseObject_in obj) = 0;
    virtual BaseObject_return get_object() = 0;
    virtual void begin_aggregate() = 0;
    virtual void end_aggregate() = 0;
};

class RequestObj_var {
protected:
    RequestObjRef _obj_;
public:
    RequestObj_var() { _obj_ = 0; };
    RequestObj_var(RequestObjRef p) { _obj_ = p; }
    RequestObj_var(const RequestObj_var& r) {
        _obj_ = RequestObj::_duplicate(r._obj_);
    }
    ~RequestObj_var() { _BaseObject__release(_obj_); }

    RequestObj_var& operator =(RequestObjRef p) {
        _BaseObject__release(_obj_);
        _obj_ = RequestObj::_duplicate(p);
        return *this;
    }
    RequestObj_var& operator =(const RequestObj_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = RequestObj::_duplicate(r._obj_);
        return *this;
    }

    RequestObjRef _obj() const { return _obj_; }
    RequestObjRef& _out() { return _obj_; }
    operator RequestObjRef() const { return _obj_; }
    RequestObjRef operator ->() const { return _obj_; }
};

inline RequestObj_var _tmp(RequestObjRef p) { return RequestObj_var(p); }


#endif
