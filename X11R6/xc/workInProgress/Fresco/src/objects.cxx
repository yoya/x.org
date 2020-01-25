/*
 * $XConsortium: objects.cxx,v 1.4 94/09/01 18:45:46 matt Exp $
 */

/*
 * Copyright (c) 1993 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Silicon Graphics and Fujitsu may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Silicon Graphics and Fujitsu.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL SILICON GRAPHICS OR FUJITSU BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * Base object, type, and exchange support.
 */

#include <X11/Fresco/Ox/env.h>
#include <X11/Fresco/Ox/exchange.h>
#include <X11/Fresco/Ox/marshal.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/transport.h>
#include <X11/Fresco/Ox/typeobjs.h>
#include <X11/Fresco/Ox/schema.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(linux) || defined(__alpha)
#include <rpc/types.h>
extern "C" void exit(int);
#endif

/* class Exchange */

Exchange::Exchange() { }
Exchange::~Exchange() { }

/* class BaseObject */

BaseObject::BaseObject() { }
BaseObject::~BaseObject() { }

TypeObj_Descriptor* _BaseObject_parents_[] = { nil };

extern TypeObjId _BaseObject_tid;

TypeObj_Descriptor _BaseObject_type = {
    /* type */ 0, /* id */ &_BaseObject_tid, "BaseObject",
    _BaseObject_parents_, /* excepts */ nil,
    /* methods */ nil, /* params */ nil, /* receive */ nil
};

Long BaseObject::ref__(Long r) { return _exchange()->ref__(r); }

Exchange* BaseObject::_exchange() { return nil; }

void* _BaseObject_tnarrow(BaseObjectRef obj, TypeObjId t, StubCreator f) {
    void* v;
    if (obj == nil) {
	v = nil;
    } else {
	Exchange* e = obj->_exchange();
	if (e != nil) {
	    v = e->narrow(t, f);
	} else {
	    v = TypeSchemaImpl::schema()->cast(obj->_this(), t, obj->_tid());
	    if (v != nil) {
		obj->ref__(+1);
	    }
	}
    }
    return v;
}

void* _BaseObject_tcast(BaseObjectRef obj, TypeObjId t) {
    return TypeSchemaImpl::schema()->cast(obj->_this(), t, obj->_tid());
}

void* BaseObject::_this() { return this; }
TypeObjId BaseObject::_tid() { return _BaseObject_tid; }

/*
 * Duplicate an object reference.  This implementation assumes
 * that objects without an exchange support reference counting
 * through the ref__ operation.
 *
 * This function is global instead of a static member function
 * to work around a compiler multiple inheritance bug.
 */

void* _BaseObject__duplicate(BaseObjectRef obj, StubCreator f) {
    BaseObjectRef r = obj;
    if (r != nil) {
	Exchange* e = r->_exchange();
	if (e == nil) {
	    r->ref__(+1);
	} else {
	    e->ref__(+1);
	    r = (*f)(e);
	}
    }
    return r;
}

/*
 * Release an object reference.  As with _duplicate, this implementation
 * assumes that an object without an exchange supports reference counting
 * through the ref__ operation.
 *
 * This function is global for the same reason as _BaseObject__duplicate.
 */

void _BaseObject__release(BaseObjectRef obj) {
    if (obj != nil) {
	Exchange* e = obj->_exchange();
	if (e == nil) {
	    if (obj->ref__(-1) <= 0) {
		delete obj;
	    }
	} else {
	    if (e->ref__(-1) <= 0) {
		delete e;
	    }
	    delete obj;
	}
    }
}

TypeObjRef BaseObject::_type() {
    return TypeSchemaImpl::schema()->map(_tid());
}

RequestObjRef BaseObject::_request() { return nil; }

/* class Env */

Env::Env() {
    raised_ = nil;
    info_ = nil;
}

Env::~Env() {
    delete raised_;
}

void Env::set_exception(Exception* e) {
    delete raised_;
    raised_ = e;
}

Boolean Env::check_exception() const {
    return raised_ != nil;
}

Exception* Env::exception_value() const {
    return raised_;
}

void Env::clear_exception() {
    delete raised_;
    raised_ = nil;
}

MethodInfo& Env::method_info() const {
    return *info_;
}

Env::ExceptionHandler Env::exception_handler(ExceptionHandler h) {
    ExceptionHandler prev = handler_;
    handler_ = h;
    return prev;
}

Env::ExceptionHandler Env::handler_;

/* class Exception */

Exception::Exception() {
    _major_ = 0;
}

Exception::~Exception() { }

void Exception::_put(MarshalBuffer& _b) const {
    _b.put_long(_major_);
}

/* class SystemException */

SystemException::SystemException() { }
SystemException::~SystemException() { }

/* class UserException */

UserException::UserException() { }
UserException::~UserException() { }

/* class TypeSchema */

TypeSchema::TypeSchema() { }
TypeSchema::~TypeSchema() { }

/* class TypeSchemaImpl */

TypeSchemaImpl* TypeSchemaImpl::schema_;

TypeSchemaImpl::TypeSchemaImpl() {
    size_ = 100;
    list_ = new TypeObj_Descriptor*[size_];
    used_ = 0;
    for (Long e = 0; e < sizeof(cache_) / sizeof(cache_[0]); e++) {
	cache_[e] = nil;
    }
}

TypeSchemaImpl::~TypeSchemaImpl() {
    delete [] list_;
}

TypeSchemaImpl* TypeSchemaImpl::schema() {
    if (schema_ == nil) {
	extern TypeObj_Descriptor** _ox_initial_schema;

	schema_ = new TypeSchemaImpl;
	schema_->load_list(_ox_initial_schema);
    }
    return schema_;
}

void TypeSchemaImpl::load(TypeObj_Descriptor* d) {
    if (used_ >= size_) {
	Long new_size = size_ + 100;
	TypeObj_Descriptor** new_list = new TypeObj_Descriptor*[new_size];
	for (Long i = 0; i < size_; i++) {
	    new_list[i] = list_[i];
	}
	delete [] list_;
	list_ = new_list;
	size_ = new_size;
    }
    list_[used_] = d;
    ++used_;
}

void TypeSchemaImpl::load_list(TypeObj_Descriptor** dd) {
    for (TypeObj_Descriptor** d = dd; *d != nil; d++) {
	load(*d);
    }
}

TypeObjRef TypeSchemaImpl::map(TypeObjId t) {
    return TypeImpl::make_type(descriptor(t));
}

void* TypeSchemaImpl::cast(
    void* obj, TypeObjId ancestor, TypeObjId descendant
) {
    return (
	descendant == ancestor ?
	    obj : match(obj, descriptor(descendant), ancestor)
    );
}

void* TypeSchemaImpl::match(void* obj, TypeObj_Descriptor* d, TypeObjId t) {
    if (d == nil) {
	return nil;
    }
    if (*d->id == t) {
	return obj;
    }
    TypeObj_Descriptor** p = d->parents;
    if (p != nil) {
	void* v = obj;
	Long* o = d->offsets;
	Long offset = 0;
	for (;;) {
	    v = match((void*)(Long(obj) + offset), *p, t);
	    if (v != nil) {
		return v;
	    }
	    ++p;
	    if (*p == nil) {
		break;
	    }
	    offset = *o;
	    ++o;
	}
    }
    return nil;
}

void TypeSchemaImpl::receiver(TypeObjId t, TypeObj_CallFunc f) {
    TypeObj_Descriptor* d = descriptor(t);
    if (d == nil) {
	fprintf(stderr, "Can't set receiver for unknown type %d\n", t);
	exit(1);
    }
    d->receive = f;
}

TypeObj_Descriptor* TypeSchemaImpl::descriptor(TypeObjId t) {
    ULong e = t & (sizeof(cache_) / sizeof(cache_[0]) - 1);
    TypeObj_Descriptor* d = cache_[e];
    if (d != nil && *d->id == t) {
	return d;
    }
    for (TypeObj_Descriptor** dd = list_; *dd != nil; dd++) {
	d = *dd;
	if (*d->id == t) {
	    cache_[e] = d;
	    return d;
	}
    }
    return nil;
}

/*
 * May need to write back dynamically-added types at some point.
 */

void TypeSchemaImpl::save() { }

void TypeSchemaImpl::deactivate() {
    save();
    delete this;
}

/* class TypeObj */

//+ TypeObj::%init
TypeObj::OpInfo::_ParamInfoSeq& TypeObj::OpInfo::_ParamInfoSeq::operator =(const _ParamInfoSeq& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new ParamInfo[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}

TypeObj::TypeObj() { }
TypeObj::~TypeObj() { }
void* TypeObj::_this() { return this; }

//+

/*
 * Need to maintain this manually to avoid Xf prefix and TypeObj stubs.
 */

TypeObj_Descriptor* _TypeObj_parents_[] = { nil };
extern TypeObjId _TypeObj_tid;
TypeObj_Descriptor _TypeObj_type = {
    /* type */ 0, /* id */ &_TypeObj_tid, "TypeObj",
    _TypeObj_parents_, /* excepts */ nil,
    /* methods */ nil, /* params */ nil, /* receive */ nil
};

TypeObjRef TypeObj::_narrow(BaseObjectRef r) {
    return (TypeObjRef)_BaseObject_tnarrow(
        r, _TypeObj_tid, 0
    );
}
TypeObjId TypeObj::_tid() { return _TypeObj_tid; }

/* class TypeImpl */

TypeImpl::TypeImpl(TypeObj::KindOf k, TypeObj_Descriptor* d) {
    kind_ = k;
    descriptor_ = d;
    d->type = this;
    if (d->methods == nil) {
	opcount_ = 0;
    } else {
	Long i;
	for (i = 0; d->methods[i].name != nil; i++);
	opcount_ = i;
    }
}

TypeImpl::~TypeImpl() { }

Long TypeImpl::ref__(Long r) { return object_.ref__(r); }

//+ TypeImpl(TypeObj::name)
string TypeImpl::name() { return descriptor_->name; }

//+ TypeImpl(TypeObj::op_info)
Boolean TypeImpl::op_info(TypeObj::OpInfo& op, Long n) {
    if (n < 0 || n >= opcount_) {
	return false;
    }
    TypeObj_Descriptor* d = descriptor_;
    if (d->methods == nil || d->params == nil) {
	return false;
    }
    const TypeObj_OpData& opdata = d->methods[n];
    op.op_name = (char*)opdata.name;
    op.op_index = n;
    op.op_result = make_type(opdata.result);
    Long nparams = opdata.nparams;
    op.op_params._length = nparams;
    op.op_params._maximum = nparams;
    delete [] op.op_params._buffer;
    op.op_params._buffer = new TypeObj::ParamInfo[nparams];
    Long p = 0;
    for (Long i = 0; i < n; i++) {
	p += d->methods[i].nparams;
    }
    for (Long j = 0; j < nparams; j++) {
	const TypeObj_ParamData& pdata = d->params[p];
	TypeObj::ParamInfo& param = op.op_params._buffer[j];
	param.param_name = (char*)pdata.name;
	param.param_mode = (TypeObj::ParamMode)pdata.mode;
	param.param_type = make_type(pdata.type);
	++p;
    }
    return true;
}

//+ TypeImpl(TypeObj::kind)
TypeObj::KindOf TypeImpl::kind() {
    return kind_;
}

//+ TypeImpl(TypeObj::enum_info)
Long TypeImpl::enum_info() {
    return 0;
}

//+ TypeImpl(TypeObj::array_info)
void TypeImpl::array_info(TypeObj_out& type, Long& size) {
    type = nil;
    size = 0;
}

//+ TypeImpl(TypeObj::members)
Long TypeImpl::members() {
    return 0;
}

/*
 * Maintain the function heading manually to avoid "unused" warning
 * for the parameter.
 */

/* TypeImpl(TypeObj::member_info) */
TypeObjRef TypeImpl::member_info(Long) { return nil; }

//+ TypeImpl(TypeObj::sequence_info)
void TypeImpl::sequence_info(TypeObj_out& type, Long& size) {
    type = nil;
    size = 0;
}

//+ TypeImpl(TypeObj::typedef_info)
TypeObj_return TypeImpl::typedef_info() {
    return nil;
}

TypeImpl* TypeImpl::make_type(TypeObj_Descriptor* d) {
    if (d == nil) {
	return nil;
    }
    TypeImpl* t = d->type;
    if (t == nil) {
	t = new TypeImpl(TypeObj::interface_type, d);
    }
    return t;
}

//+ RequestObj::%init
RequestObj::RequestObj() { }
RequestObj::~RequestObj() { }
void* RequestObj::_this() { return this; }

//+

/*
 * This has to be maintained manually to avoid generating
 * Xf names for the Ox basic types.
 */

TypeObj_Descriptor* _RequestObj_parents_[] = { nil };
extern TypeObjId _RequestObj_tid;
TypeObj_Descriptor _RequestObj_type = {
    /* type */ 0, /* id */ &_RequestObj_tid, "RequestObj",
    _RequestObj_parents_, /* excepts */ nil,
    /* methods */ nil, /* params */ nil, /* receive */ nil
};

RequestObjRef RequestObj::_narrow(BaseObjectRef o) {
    return (RequestObjRef)_BaseObject_tnarrow(
        o, _RequestObj_tid, 0
    );
}
TypeObjId RequestObj::_tid() { return _RequestObj_tid; }

RequestObjImpl::RequestObjImpl(BaseObjectRef o) {
    target_ = BaseObject::_duplicate(o);
    init();
}

RequestObjImpl::~RequestObjImpl() {
    target_->ref__(-1);
    free();
}

Long RequestObjImpl::ref__(Long r) { return object_.ref__(r); }

//+ RequestObjImpl(RequestObj::set_operation)
void RequestObjImpl::set_operation(string s) {
    free();
    init();
    operation_ = new char[strlen(s) + 1];
    strcpy(operation_, s);
}

//+ RequestObjImpl(RequestObj::invoke)
RequestObj::CallStatus RequestObjImpl::invoke() {
    if (resolved_ == 0) {
	return status(RequestObj::unknown_operation);
    }
    if (status_ == RequestObj::ambiguous_operation) {
	return status_;
    }
    if (argc_ != opinfo_.op_params._length) {
	return status(RequestObj::bad_argument_count);
    }
    buffer_->make_out_in();
    buffer_->dispatch(target_);
    buffer_->make_out_in();
    if (buffer_->get_unsigned_long() != 0) {
	return status(RequestObj::invoke_failed);
    }
    return status(RequestObj::ok);
}

//+ RequestObjImpl(RequestObj::op_info)
RequestObj::CallStatus RequestObjImpl::op_info(TypeObj::OpInfo& op) {
    if (resolved_ == 0) {
	status_ = RequestObj::unknown_operation;
	TypeObjRef type = target_->_type();
	if (find_op(type, index_)) {
	    resolved_ = target_->_tid();
	    status_ = RequestObj::ok;
	} else {
	    resolve(TypeSchemaImpl::schema()->descriptor(target_->_tid()));
	}
	if (is_ok()) {
	    init_buffer();
	}
    }
    if (is_ok()) {
	TypeObjRef type = TypeSchemaImpl::schema()->map(resolved_);
	type->op_info(op, index_);
    }
    return status_;
}

RequestObj::CallStatus RequestObjImpl::status(RequestObj::CallStatus s) {
    status_ = s;
    return s;
}

//+ RequestObjImpl(RequestObj::put_char)
void RequestObjImpl::put_char(Char value) {
    buffer_->put_char(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_char)
Char RequestObjImpl::get_char() {
    return buffer_->get_char();
}

//+ RequestObjImpl(RequestObj::put_boolean)
void RequestObjImpl::put_boolean(Boolean value) {
    buffer_->put_boolean(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_boolean)
Boolean RequestObjImpl::get_boolean() {
    return buffer_->get_boolean();
}

//+ RequestObjImpl(RequestObj::put_octet)
void RequestObjImpl::put_octet(Octet value) {
    buffer_->put_octet(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_octet)
Octet RequestObjImpl::get_octet() {
    return buffer_->get_octet();
}

//+ RequestObjImpl(RequestObj::put_short)
void RequestObjImpl::put_short(Short value) {
    buffer_->put_short(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_short)
Short RequestObjImpl::get_short() {
    return buffer_->get_short();
}

//+ RequestObjImpl(RequestObj::put_unsigned_short)
void RequestObjImpl::put_unsigned_short(UShort value) {
    buffer_->put_unsigned_short(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_unsigned_short)
UShort RequestObjImpl::get_unsigned_short() {
    return buffer_->get_unsigned_short();
}

//+ RequestObjImpl(RequestObj::put_long)
void RequestObjImpl::put_long(Long value) {
    buffer_->put_long(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_long)
Long RequestObjImpl::get_long() {
    return buffer_->get_long();
}

//+ RequestObjImpl(RequestObj::put_unsigned_long)
void RequestObjImpl::put_unsigned_long(ULong value) {
    buffer_->put_unsigned_long(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_unsigned_long)
ULong RequestObjImpl::get_unsigned_long() {
    return buffer_->get_unsigned_long();
}

//+ RequestObjImpl(RequestObj::put_float)
void RequestObjImpl::put_float(Float value) {
    buffer_->put_float(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_float)
Float RequestObjImpl::get_float() {
    return buffer_->get_float();
}

//+ RequestObjImpl(RequestObj::put_double)
void RequestObjImpl::put_double(Double value) {
    buffer_->put_double(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_double)
Double RequestObjImpl::get_double() {
    return buffer_->get_double();
}

//+ RequestObjImpl(RequestObj::put_string)
void RequestObjImpl::put_string(string value) {
    buffer_->put_string(value);
    put();
}

//+ RequestObjImpl(RequestObj::get_string)
string RequestObjImpl::get_string() {
    return buffer_->get_string();
}

//+ RequestObjImpl(RequestObj::put_object)
void RequestObjImpl::put_object(BaseObject_in obj) {
    buffer_->put_object(obj);
    put();
}

//+ RequestObjImpl(RequestObj::get_object)
BaseObject_return RequestObjImpl::get_object() {
    return buffer_->get_object(nil);
}

//+ RequestObjImpl(RequestObj::begin_aggregate)
void RequestObjImpl::begin_aggregate() {
    ++nesting_;
}

//+ RequestObjImpl(RequestObj::end_aggregate)
void RequestObjImpl::end_aggregate() {
    if (nesting_ > 0) {
	--nesting_;
	put();
    }
}

void RequestObjImpl::put() {
    if (nesting_ == 0) {
	++argc_;
    }
}

void RequestObjImpl::init() {
    operation_ = nil;
    index_ = -1;
    resolved_ = 0;
    status_ = RequestObj::initial;
    buffer_ = new MarshalBuffer;
    buffer_->reset();
    buffer_->env(nil);
    argc_ = 0;
    nesting_ = 0;
}

void RequestObjImpl::free() {
    delete operation_;
    delete buffer_;
}

Boolean RequestObjImpl::find_op(TypeObjRef type, Long& index) {
    Boolean b = false;
    TypeObj::OpInfo op;
    for (Long i = 0; type->op_info(op, i); i++) {
	if (strcmp(operation_, op.op_name) == 0) {
	    index = i;
	    opinfo_ = op;
	    op.op_params._buffer = nil;
	    b = true;
	    break;
	} else {
	    delete [] op.op_params._buffer;
	    op.op_params._buffer = nil;
	}
    }
    return b;
}

void RequestObjImpl::resolve(TypeObj_Descriptor* d) {
    TypeObj_Descriptor** p = d->parents;
    if (p != nil) {
	for (; *p != nil; p++) {
	    TypeObjRef type = TypeImpl::make_type(*p);
	    Long index;
	    if (find_op(type, index)) {
		TypeObjId tid = *(*p)->id;
		if (resolved_ == 0 || resolved_ == tid) {
		    resolved_ = tid;
		    index_ = index;
		    status_ = RequestObj::ok;
		    /*
		     * Found first occurrence.  Now continue search in siblings
		     * for ambiguities.
		     */
		} else {
		    status_ = RequestObj::ambiguous_operation;
		    return;
		}
	    } else {
		resolve(*p);
	    }
	}
    }
}

void RequestObjImpl::init_buffer() {
    buffer_->put_unsigned_long(resolved_);
    buffer_->put_long(index_);
}

/* Basic types */

#define ImplementBasicType(T,K,V,TSize,TString) \
extern TypeObjId TypeIdVar(V); \
DefineType(V, TSize, TString) \
T::T() : TypeImpl(TypeObj::K, &TypeVar(V)) { } \
T::~T() { }

ImplementBasicType(VoidTypeObj, void_type, Xfvoid, Long, "void")
ImplementBasicType( \
    BooleanTypeObj, boolean_type, XfBoolean, Boolean, "boolean" \
)
ImplementBasicType(CharTypeObj, char_type, XfChar, char, "char")
ImplementBasicType(OctetTypeObj, octet_type, XfOctet, Octet, "octet")
ImplementBasicType(ShortTypeObj, short_type, XfShort, short, "short")
ImplementBasicType( \
    UShortTypeObj, unsigned_short_type, XfUShort, \
    unsigned short, "unsigned short" \
)
ImplementBasicType(LongTypeObj, long_type, XfLong, Long, "long")
ImplementBasicType( \
    ULongTypeObj, unsigned_long_type, XfULong, ULong, "unsigned long" \
)
ImplementBasicType( \
    LongLongTypeObj, longlong_type, XfLongLong, LongLong, "longlong" \
)
ImplementBasicType( \
    ULongLongTypeObj, unsigned_longlong_type, XfULongLong, ULongLong, \
    "unsigned longlong" \
)
ImplementBasicType(FloatTypeObj, float_type, XfFloat, float, "float")
ImplementBasicType(DoubleTypeObj, double_type, XfDouble, double, "double")
ImplementBasicType(StringTypeObj, string_type, Xfstring, char*, "string")

/* class EnumTypeObj */

EnumTypeObj::EnumTypeObj(
    TypeObj_Descriptor* d, ULong maximum
) : TypeImpl(TypeObj::enum_type, d) {
    max_ = maximum;
}

EnumTypeObj::~EnumTypeObj() { }

//+ EnumTypeObj(TypeObj::enum_info)
Long EnumTypeObj::enum_info() {
    return max_;
}

/* class ArrayTypeObj */

ArrayTypeObj::ArrayTypeObj(
    TypeObj_Descriptor* d, TypeObj_Descriptor* element, Long count
) : TypeImpl(TypeObj::array_type, d) {
    type_ = element;
    count_ = count;
}

ArrayTypeObj::~ArrayTypeObj() { }

//+ ArrayTypeObj(TypeObj::array_info)
void ArrayTypeObj::array_info(TypeObj_out& type, Long& size) {
    type = TypeObj::_duplicate(TypeImpl::make_type(type_));
    size = count_;
}

/* class StructTypeObj */

StructTypeObj::StructTypeObj(
    TypeObj_Descriptor* d, TypeObj_Descriptor** members
) : TypeImpl(TypeObj::struct_type, d) {
    members_ = members;
    count_ = 0;
    TypeObj_Descriptor** m;
    for (m = members_; *m != nil; m++, count_++);
}

StructTypeObj::~StructTypeObj() { }

//+ StructTypeObj(TypeObj::members)
Long StructTypeObj::members() {
    return count_;
}

//+ StructTypeObj(TypeObj::member_info)
TypeObj_return StructTypeObj::member_info(Long n) {
    if (n < 0 || n >= count_) {
	return nil;
    }
    return TypeObj::_duplicate(TypeImpl::make_type(members_[n]));
}

/* class SequenceTypeObj */

struct GenericSequence {
    Long maximum;
    Long length;
    void* buffer;
};

SequenceTypeObj::SequenceTypeObj(
    TypeObj_Descriptor* d, TypeObj_Descriptor* element
) : TypeImpl(TypeObj::sequence_type, d) {
    type_ = element;
}

SequenceTypeObj::~SequenceTypeObj() { }

//+ SequenceTypeObj(TypeObj::sequence_info)
void SequenceTypeObj::sequence_info(TypeObj_out& type, Long& size) {
    type = TypeObj::_duplicate(TypeImpl::make_type(type_));
    size = 0;
}

/* class TypedefObj */

TypedefObj::TypedefObj(
    TypeObj_Descriptor* d, TypeObj_Descriptor* type
) : TypeImpl(TypeObj::typedef_type, d) {
    type_ = type;
}

TypedefObj::~TypedefObj() { }

//+ TypedefObj(TypeObj::typedef_info)
TypeObj_return TypedefObj::typedef_info() {
    return TypeObj::_duplicate(TypeImpl::make_type(type_));
}

/* class RemoteExchange */

RemoteExchange::RemoteExchange(
    char* host, ULong port, ULong impl, ULong obj
) {
    hostname_ = host;
    port_ = port;
    impl_ = impl;
    obj_ = obj;
}

RemoteExchange::RemoteExchange(const RemoteExchange& e) {
    /* copy string data? */
    hostname_ = e.hostname_;

    port_ = e.port_;
    impl_ = e.impl_;
    obj_ = e.obj_;
}

RemoteExchange::~RemoteExchange() { }

Long RemoteExchange::ref__(Long r) { return object_.ref__(r); }

void RemoteExchange::op(MarshalBuffer& b, TypeObjId t, Long op) {
    b.put_unsigned_long(impl_);
    b.put_unsigned_long(obj_);
    b.put_unsigned_long(t);
    b.put_long(op);
}

Long RemoteExchange::invoke(MarshalBuffer& b, Boolean /* oneway */) {
    int r = oxCall(hostname_, port_, &b);
    if (r != 0) {
	fprintf(stderr, "oxCall returns %d\n", r);
	exit(1);
    }
    return b.get_long();
}

void RemoteExchange::marshal(MarshalBuffer& b) {
    extern ExchangeId _RemoteExchange_xid_;
    b.put_unsigned_long(_RemoteExchange_xid_);
    b.put_string(hostname_, -1);
    b.put_unsigned_long(port_);
    b.put_unsigned_long(impl_);
    b.put_unsigned_long(obj_);
}

BaseObjectRef _RemoteExchange_unmarshal(StubCreator f, MarshalBuffer& b) {
    char* host = b.get_string();
    ULong port = b.get_unsigned_long();
    ULong impl = b.get_unsigned_long();
    ULong obj = b.get_unsigned_long();
    /* should do lookup to avoid creating same object twice in client */
    return (*f)(new RemoteExchange(host, port, impl, obj));
}

BaseObjectRef RemoteExchange::narrow(TypeObjId t, StubCreator f) {
    MarshalBuffer buf;
    buf.reset();
    op(buf, t, RemoteExchange_narrow_op);
    Long r = invoke(buf, false);
    if (r != 0) {
	fprintf(stderr, "Unexpected reply %d for _narrow\n", r);
	exit(1);
    }
    return (buf.get_long() == 0) ? nil : (*f)(new RemoteExchange(*this));
}

/* Library exchange */

BaseObjectRef _LibraryExchange_unmarshal(StubCreator, MarshalBuffer& b) {
    return (BaseObjectRef)b.get_unsigned_long();
}

/* Standard exchanges */

BaseObjectRef (*_exchange_unmarshal_[])(StubCreator, MarshalBuffer&) = {
    nil,
    nil, /* nil xid */
    &_RemoteExchange_unmarshal,
    &_LibraryExchange_unmarshal,
    nil
};

/* class MarshalBuffer */

#define buf_words(n) (((n) + sizeof(Long) - 1) / sizeof(Long))

void MarshalBuffer::reset() {
    cur_ = &data_[0];
    end_ = &data_[sizeof(data_) / sizeof(data_[0])];
}

void MarshalBuffer::make_out_in() {
    end_ = cur_;
    cur_ = &data_[0];
}

void MarshalBuffer::put32(Long n) {
    if (cur_ >= end_) {
	overflow();
    }
    *cur_++ = n;
}

Long MarshalBuffer::get32() {
    if (cur_ >= end_) {
	underflow();
    }
    long n = *cur_;
    ++cur_;
    return n;
}

void MarshalBuffer::overflow() {
    fprintf(stderr, "MarshalBuffer overflow\n");
    exit(1);
}

void MarshalBuffer::underflow() {
    fprintf(stderr, "MarshalBuffer underflow\n");
    exit(1);
}

void MarshalBuffer::put_boolean(Boolean b) { put32(Long(b)); }
void MarshalBuffer::put_char(char c) { put32(Long(c)); }
void MarshalBuffer::put_octet(unsigned char c) { put32(Long(c)); }
void MarshalBuffer::put_short(short s) { put32(Long(s)); }
void MarshalBuffer::put_unsigned_short(unsigned short s) {
    put32(Long(s));
}
void MarshalBuffer::put_long(Long n) { put32(n); }
void MarshalBuffer::put_unsigned_long(ULong n) { put32(Long(n)); }

void MarshalBuffer::put_longlong(LongLong n) {
    Long w = buf_words(sizeof(n));
    if (cur_ + w >= end_) {
	overflow();
    }
    *(LongLong*)cur_ = n;
    move(w);
}

void MarshalBuffer::put_unsigned_longlong(ULongLong n) {
    Long w = buf_words(sizeof(n));
    if (cur_ + w >= end_) {
	overflow();
    }
    *(ULongLong*)cur_ = n;
    move(w);
}

void MarshalBuffer::put_float(float f) {
    Long w = buf_words(sizeof(f));
    if (cur_ + w >= end_) {
	overflow();
    }
    *(float*)cur_ = f;
    move(w);
}

void MarshalBuffer::put_double(double d) {
    Long w = buf_words(sizeof(d));
    if (cur_ + w >= end_) {
	overflow();
    }
    *(double*)cur_ = d;
    move(w);
}

void MarshalBuffer::put_string(const char* s, Long length) {
    char* p = (char*)cur_;
    long n = (length == -1) ? strlen(s) : length;
    long n1 = n + 1;
    if (p + n1 >= (char*)end_) {
	overflow();
    }
    strncpy(p, s, int(n));
    p[n] = '\0';
    move(n1);
}

void MarshalBuffer::put_seq_hdr(const void* v) {
    const GenericSequence* seq = (const GenericSequence*)v;
    put_long(seq->maximum);
    put_long(seq->length);
}

/*
 * This function assumes the sequence element type is primitive,
 * e.g., char, short, long.
 */

void MarshalBuffer::put_seq(const void* v, Long size) {
    const GenericSequence* seq = (const GenericSequence*)v;
    put_long(seq->maximum);
    put_long(seq->length);
    Long n = seq->length * size;
    Long w = buf_words(n);
    if (cur_ + w >= end_) {
	overflow();
    }
    memcpy(cur_, seq->buffer, int(n));
    move(w);
}

void MarshalBuffer::put_object(BaseObjectRef o) {
    extern ExchangeId _nilref_xid_, _LibraryExchange_xid_;
    if (is_nil(o)) {
	put_unsigned_long(_nilref_xid_);
    } else {
	Exchange* e = o->_exchange();
	if (e != nil) {
	    e->marshal(*this);
	} else {
	    /* Use library exchange if object doesn't otherwise have one */
	    put_unsigned_long(_LibraryExchange_xid_);
	    put_unsigned_long((ULong)BaseObject::_duplicate(o));
	}
    }
}

Boolean MarshalBuffer::get_boolean() { return Boolean(get32()); }
char MarshalBuffer::get_char() { return char(get32()); }
unsigned char MarshalBuffer::get_octet() { return (unsigned char)(get32()); }
short MarshalBuffer::get_short() { return short(get32()); }
unsigned short MarshalBuffer::get_unsigned_short() {
    return (unsigned short)(get32());
}
Long MarshalBuffer::get_long() { return Long(get32()); }
ULong MarshalBuffer::get_unsigned_long() {
    return (ULong)get32();
}

LongLong MarshalBuffer::get_longlong() {
    LongLong n = *(LongLong*)cur_;
    move(buf_words(sizeof(n)));
    return n;
}

ULongLong MarshalBuffer::get_unsigned_longlong() {
    ULongLong n = *(ULongLong*)cur_;
    move(buf_words(sizeof(n)));
    return n;
}

float MarshalBuffer::get_float() {
    float f = *(float*)cur_;
    move(buf_words(sizeof(f)));
    return f;
}

double MarshalBuffer::get_double() {
    double d = *(double*)cur_;
    move(buf_words(sizeof(d)));
    return d;
}

char* MarshalBuffer::get_string() {
    char* p = (char*)cur_;
    long length = strlen(p) + 1;
    char* s = new char[length];
    strcpy(s, p);
    move(length);
    return s;
}

void MarshalBuffer::get_seq_hdr(void* v) {
    GenericSequence* seq = (GenericSequence*)v;
    seq->maximum = get_long();
    seq->length = get_long();
}

/*
 * This function assumes the sequence element type is primitive,
 * e.g., char, short, long.
 */

void MarshalBuffer::get_seq(void* v, Long size) {
    GenericSequence* seq = (GenericSequence*)v;
    seq->maximum = get_long();
    seq->length = get_long();
    Long n = seq->length * size;
    Long w = buf_words(n);
    if (cur_ + w >= end_) {
	underflow();
    }
    seq->buffer = new char[n];
    memcpy(seq->buffer, cur_, int(n));
    move(w);
}

BaseObjectRef MarshalBuffer::get_object(StubCreator f) {
    extern ExchangeId _nilref_xid_, _maximum_known_xid_;
    ExchangeId x = get_unsigned_long();
    if (x == 0 || x > _maximum_known_xid_) {
	fprintf(stderr, "Bad exchange id (%d)\n", x);
	exit(1);
    }
    return (x == _nilref_xid_) ? nil : (*_exchange_unmarshal_[x])(f, *this);
}

/*
 * Offset table for moving the function table index.  Parameters
 * with type _t_addr get two entries, one for put and one for get,
 * while those with type _t_object get one entry for the stub creator.
 * All other types do not have an entry.
 */
static unsigned char func_delta[] = {
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
};

/*
 * Perform a remote call using the object's exchange to setup the
 * buffer and do the actual invocation.  The argument descriptors
 * provide the necessary information to marshal in/inout parameters and
 * unmarshal out/inout parameters and a return value.
 */

void MarshalBuffer::invoke(
    BaseObjectRef obj, ArgInfo& info, ArgValue* value, Env* e
) {
    Exchange* exch = obj->_exchange();
    if (exch != nil) {
	reset();
	env(e);
	exch->op(*this, *info.id, info.method);
	Long i;
	Long argc = info.desc[0];
	ArgDesc rdesc = info.desc[1] >> 2;
	ArgDesc* dp = &info.desc[2];
	/* marshal functions are put/get pairs */
	ArgMarshal* fp = &info.func[0];
	ArgValue* ap = &value[1];
	for (i = 1; i < argc; i++, dp++, ap++) {
	    ArgDesc d = *dp;
	    ArgDesc t = d >> 2;
	    switch (d & 0x3) {
	    case /* in */ 1:
		put_in_param(t, fp, ap);
		break;
	    case /* inout */ 3:
		put_inout_param(t, fp, ap);
		break;
	    }
	    fp += func_delta[t];
	}
	Long r = exch->invoke(*this, rdesc == _t_void_oneway);
	if (r == 0) {
	    /* normal return */
	    dp = &info.desc[2];
	    fp = &info.func[0];
	    ap = &value[1];
	    for (i = 1; i < argc; i++, dp++, ap++) {
		ArgDesc d = *dp;
		ArgDesc t = d >> 2;
		if ((d & 0x2) != 0) {
		    /* out or inout parameter */
		    get_param(t, fp, ap);
		}
		fp += func_delta[t];
	    }
	    get_result(rdesc, fp, value);
	} else {
	    /* raised exception */
	    TypeObj_Descriptor* t = TypeSchemaImpl::schema()->descriptor(r);
	    Long index = get_long();
	    if (t == nil) {
		/* should raise system exception */
		fprintf(stderr, "Bad exception tid %d\n", r);
		exit(1);
	    } else {
		Env* ev = env();
		if (ev == nil) {
		    /* use some default environment */
		    return;
		}
		ev->set_exception((*t->excepts[index - 1])(*this));
	    }
	}
    }
}

void MarshalBuffer::put_in_param(ArgDesc d, ArgMarshal* f, ArgValue* a) {
    switch (d) {
    case _t_addr: (*MarshalFunc(f[0]))(this, a->u_vaddr); break;
    case _t_void: /* shouldn't happen */ break;
    case _t_void_oneway: /* shouldn't happen */ break;
    case _t_boolean: put_boolean(a->u_boolean); break;
    case _t_char: put_char(a->u_char); break;
    case _t_octet: put_octet(a->u_octet); break;
    case _t_short: put_short(a->u_short); break;
    case _t_unsigned_short: put_unsigned_short(a->u_unsigned_short); break;
    case _t_long: put_long(a->u_long); break;
    case _t_unsigned_long: put_unsigned_long(a->u_unsigned_long); break;
    case _t_longlong: put_longlong(a->u_longlong); break;
    case _t_unsigned_longlong:
	put_unsigned_longlong(a->u_unsigned_longlong);
	break;
    case _t_float: put_float(a->u_float); break;
    case _t_double: put_double(a->u_double); break;
    case _t_string: put_string(a->u_string); break;
    case _t_object: put_object(a->u_objref); break;
    }
}

void MarshalBuffer::put_inout_param(ArgDesc d, ArgMarshal* f, ArgValue* a) {
    void* p = a->u_vaddr;
    switch (d) {
    case _t_addr: (*MarshalFunc(f[0]))(this, p); break;
    case _t_void: /* shouldn't happen */ break;
    case _t_void_oneway: /* shouldn't happen */ break;
    case _t_boolean: put_boolean(*(Boolean*)p); break;
    case _t_char: put_char(*(char*)p); break;
    case _t_octet: put_octet(*(unsigned char*)p); break;
    case _t_short: put_short(*(short*)p); break;
    case _t_unsigned_short: put_unsigned_short(*(unsigned short*)p); break;
    case _t_long: put_long(*(Long*)p); break;
    case _t_unsigned_long: put_unsigned_long(*(ULong*)p); break;
    case _t_longlong: put_longlong(*(LongLong*)p); break;
    case _t_unsigned_longlong: put_unsigned_longlong(*(ULongLong*)p); break;
    case _t_float: put_float(*(float*)p); break;
    case _t_double: put_double(*(double*)p); break;
    case _t_string: put_string(*(char**)p); break;
    case _t_object: put_object(*(BaseObjectRef*)p); break;
    }
}

void MarshalBuffer::get_param(ArgDesc d, ArgMarshal* f, ArgValue* a) {
    void* p = a->u_vaddr;
    switch (d) {
    case _t_addr: (*MarshalFunc(f[1]))(this, p); break;
    case _t_void: /* shouldn't happen */ break;
    case _t_void_oneway: /* shouldn't happen */ break;
    case _t_boolean: *(Boolean*)p = get_boolean(); break;
    case _t_char: *(char*)p = get_char(); break;
    case _t_octet: *(unsigned char*)p = get_octet(); break;
    case _t_short: *(short*)p = get_short(); break;
    case _t_unsigned_short: *(unsigned short*)p = get_unsigned_short(); break;
    case _t_long: *(Long*)p = get_long(); break;
    case _t_unsigned_long: *(ULong*)p = get_unsigned_long(); break;
    case _t_longlong: *(LongLong*)p = get_longlong(); break;
    case _t_unsigned_longlong: *(ULongLong*)p = get_unsigned_longlong(); break;
    case _t_float: *(float*)p = get_float(); break;
    case _t_double: *(double*)p = get_double(); break;
    case _t_string: *(char**)p = get_string(); break;
    case _t_object: *(BaseObjectRef*)p = get_object(StubCreator(*f)); break;
    }
}

void MarshalBuffer::get_result(ArgDesc d, ArgMarshal* f, ArgValue* a) {
    switch (d) {
    case _t_addr: (*MarshalFunc(f[1]))(this, a->u_vaddr); break;
    case _t_void: /* function with no return value */ break;
    case _t_void_oneway: /* shouldn't happen */ break;
    case _t_boolean: a->u_boolean = get_boolean(); break;
    case _t_char: a->u_char = get_char(); break;
    case _t_octet: a->u_octet = get_octet(); break;
    case _t_short: a->u_short = get_short(); break;
    case _t_unsigned_short: a->u_unsigned_short = get_unsigned_short(); break;
    case _t_long: a->u_long = get_long(); break;
    case _t_unsigned_long: a->u_unsigned_long = get_unsigned_long(); break;
    case _t_longlong: a->u_longlong = get_longlong(); break;
    case _t_unsigned_longlong:
	a->u_unsigned_longlong = get_unsigned_longlong();
	break;
    case _t_float: a->u_float = get_float(); break;
    case _t_double: a->u_double = get_double(); break;
    case _t_string: a->u_string = get_string(); break;
    case _t_object: a->u_objref = get_object(StubCreator(*f)); break;
    }
}

/*
 * Before calling the implementation, unmarshal the in/inout parameters.
 */

void MarshalBuffer::receive(ArgInfo& info, ArgValue* value) {
    Long i;
    Long argc = info.desc[0];
    ArgDesc* dp = &info.desc[2];
    ArgMarshal* fp = &info.func[0];
    ArgValue* ap = &value[1];
    for (i = 1; i < argc; i++, dp++, ap++) {
	ArgDesc d = *dp;
	ArgDesc t = d >> 2;
	if ((d & 0x1) != 0) {
	    /* in or inout parameter */
	    get_param(t, fp, ap);
	}
	fp += func_delta[t];
    }
}

/*
 * After calling the implementation, marshal the out/inout parameters and
 * return value.
 */

void MarshalBuffer::reply(ArgInfo& info, ArgValue* value) {
    reset();
    if (env_ != nil && env_->check_exception()) {
	const Exception* e = env_->exception_value();
	e->_put(*this);
	env_->clear_exception();
	return;
    }
    put_long(0);
    Long i;
    Long argc = info.desc[0];
    /* desc[1] describes return value */
    ArgDesc* dp = &info.desc[2];
    ArgMarshal* fp = &info.func[0];
    ArgValue* ap = &value[1];
    for (i = 1; i < argc; i++, dp++, ap++) {
	ArgDesc d = *dp;
	ArgDesc t = d >> 2;
	if ((d & 0x2) != 0) {
	    /* out or inout parameter */
	    put_inout_param(t, fp, ap);
	}
	fp += func_delta[t];
    }
    put_in_param(info.desc[1] >> 2, fp, value);
}

void MarshalBuffer::dispatch(BaseObjectRef obj) {
    TypeSchemaImpl* s = TypeSchemaImpl::schema();
    TypeObjId t = get_unsigned_long();
    TypeObj_Descriptor* d = s->descriptor(t);
    if (d == nil) {
	fprintf(stderr, "Unknown type %d in dispatch\n", t);
	exit(1);
    }
    TypeObj_CallFunc f = d->receive;
    if (f == nil) {
	fprintf(stderr, "No receive function for type '%s'\n", d->name);
	exit(1);
    }
    ULong m = get_unsigned_long();
    if (m == RemoteExchange_narrow_op) {
	reset();
	put_long(0);
	put_long(s->match(obj, d, obj->_tid()) != nil);
    } else {
	(*d->receive)(obj, m, *this);
    }
}

/* class BaseObjectStub */

BaseObjectStub::BaseObjectStub(Exchange* e) { exch_ = e; }
BaseObjectStub::~BaseObjectStub() { delete exch_; }

BaseObjectRef _BaseObjectStub_create(Exchange* e) {
    return new BaseObjectStub(e);
}

Exchange* BaseObjectStub::_exchange() { return exch_; }
