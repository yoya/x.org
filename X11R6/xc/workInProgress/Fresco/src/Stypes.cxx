/*
 * $XConsortium: Stypes.cxx,v 1.2 94/04/01 16:48:14 matt Exp $
 */

/*
 * Copyright (c) 1992-93 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
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
 * Stubs for basic types
 */

#include <X11/Fresco/types.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/schema.h>

//+ Action::%init,type+dii,client
Action::Action() { }
Action::~Action() { }
void* Action::_this() { return this; }

extern TypeObj_Descriptor _Xfvoid_type, _XfBoolean_type;

TypeObj_OpData _XfAction_methods[] = {
    { "execute", &_Xfvoid_type, 0 },
    { "reversible", &_XfBoolean_type, 0 },
    { "unexecute", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfAction_params[] = {
    { 0, 0, 0 }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfAction_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfAction_tid;
extern void _XfAction_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfAction_type = {
    /* type */ 0,
    /* id */ &_XfAction_tid,
    "Action",
    _XfAction_parents, /* offsets */ nil, /* excepts */ nil,
    _XfAction_methods, _XfAction_params,
    &_XfAction_receive
};

ActionRef Action::_narrow(BaseObjectRef o) {
    return (ActionRef)_BaseObject_tnarrow(
        o, _XfAction_tid, &_XfActionStub_create
    );
}
TypeObjId Action::_tid() { return _XfAction_tid; }
void _XfAction_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfAction_tid;
    ActionRef _this = (ActionRef)_BaseObject_tcast(_object, _XfAction_tid);
    switch (_m) {
        case /* execute */ 0: {
            extern MarshalBuffer::ArgInfo _XfAction_execute_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->execute();
            _b.reply(_XfAction_execute_pinfo, _arg);
            break;
        }
        case /* reversible */ 1: {
            extern MarshalBuffer::ArgInfo _XfAction_reversible_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->reversible();
            _b.reply(_XfAction_reversible_pinfo, _arg);
            break;
        }
        case /* unexecute */ 2: {
            extern MarshalBuffer::ArgInfo _XfAction_unexecute_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->unexecute();
            _b.reply(_XfAction_unexecute_pinfo, _arg);
            break;
        }
    }
}
ActionStub::ActionStub(Exchange* e) { exch_ = e; }
ActionStub::~ActionStub() { }
BaseObjectRef _XfActionStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new ActionStub(e);
}
Exchange* ActionStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfAction_execute_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAction_execute_pinfo = {
    &_XfAction_tid, 0, _XfAction_execute_pdesc, 0
};
void Action::execute() {
    MarshalBuffer _b;
    extern TypeObjId _XfAction_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAction_execute_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfAction_reversible_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfAction_reversible_pinfo = {
    &_XfAction_tid, 1, _XfAction_reversible_pdesc, 0
};
Boolean Action::reversible() {
    MarshalBuffer _b;
    extern TypeObjId _XfAction_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAction_reversible_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfAction_unexecute_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfAction_unexecute_pinfo = {
    &_XfAction_tid, 2, _XfAction_unexecute_pdesc, 0
};
void Action::unexecute() {
    MarshalBuffer _b;
    extern TypeObjId _XfAction_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfAction_unexecute_pinfo, _arg);
}
//+

//+ CharString::%init,type,client
CharString::Data& CharString::Data::operator =(const Data& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new CharCode[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}
CharString::CharData& CharString::CharData::operator =(const CharData& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new Char[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}

CharString::CharString() { }
CharString::~CharString() { }
void* CharString::_this() { return this; }

extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfCharString_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfCharString_tid;
TypeObj_Descriptor _XfCharString_type = {
    /* type */ 0,
    /* id */ &_XfCharString_tid,
    "CharString",
    _XfCharString_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

CharStringRef CharString::_narrow(BaseObjectRef o) {
    return (CharStringRef)_BaseObject_tnarrow(
        o, _XfCharString_tid, &_XfCharStringStub_create
    );
}
TypeObjId CharString::_tid() { return _XfCharString_tid; }
extern void _XfCharString_Data_put(
    MarshalBuffer&, const CharString::Data&
);
extern void _XfCharString_Data_get(
    MarshalBuffer&, CharString::Data&
);
extern void _XfCharString_CharData_put(
    MarshalBuffer&, const CharString::CharData&
);
extern void _XfCharString_CharData_get(
    MarshalBuffer&, CharString::CharData&
);
extern void _XfCharString_Data_put(
    MarshalBuffer&, const CharString::Data&
);
extern void _XfCharString_Data_get(
    MarshalBuffer&, CharString::Data&
);
extern void _XfCharString_CharData_put(
    MarshalBuffer&, const CharString::CharData&
);
extern void _XfCharString_CharData_get(
    MarshalBuffer&, CharString::CharData&
);
extern void _XfCharString_Data_put(
    MarshalBuffer&, const CharString::Data&
);
extern void _XfCharString_Data_get(
    MarshalBuffer&, CharString::Data&
);
extern void _XfCharString_CharData_put(
    MarshalBuffer&, const CharString::CharData&
);
extern void _XfCharString_CharData_get(
    MarshalBuffer&, CharString::CharData&
);

CharStringStub::CharStringStub(Exchange* e) { exch_ = e; }
CharStringStub::~CharStringStub() { }
BaseObjectRef _XfCharStringStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new CharStringStub(e);
}
Exchange* CharStringStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfCharString_hash_pdesc[2] = { 1, 36 };
MarshalBuffer::ArgInfo _XfCharString_hash_pinfo = {
    &_XfCharString_tid, 0, _XfCharString_hash_pdesc, 0
};
CharString::HashValue CharString::hash() {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfCharString_hash_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfCharString_count_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfCharString_count_pinfo = {
    &_XfCharString_tid, 1, _XfCharString_count_pdesc, 0
};
Long CharString::count() {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfCharString_count_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfCharString_copy_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfCharString_copy_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfCharString_copy_pinfo = {
    &_XfCharString_tid, 2, _XfCharString_copy_pdesc, _XfCharString_copy_pfunc
};
void CharString::copy(CharString_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfCharString_copy_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfCharString_equal_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfCharString_equal_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfCharString_equal_pinfo = {
    &_XfCharString_tid, 3, _XfCharString_equal_pdesc, _XfCharString_equal_pfunc
};
Boolean CharString::equal(CharString_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfCharString_equal_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfCharString_case_insensitive_equal_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfCharString_case_insensitive_equal_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfCharString_case_insensitive_equal_pinfo = {
    &_XfCharString_tid, 4, _XfCharString_case_insensitive_equal_pdesc, _XfCharString_case_insensitive_equal_pfunc
};
Boolean CharString::case_insensitive_equal(CharString_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfCharString_case_insensitive_equal_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfCharString_get_data_pdesc[3] = { 2, 4, 3 };
MarshalBuffer::ArgMarshal _XfCharString_get_data_pfunc[] = {
    &_XfCharString_Data_put, &_XfCharString_Data_get,

};
MarshalBuffer::ArgInfo _XfCharString_get_data_pinfo = {
    &_XfCharString_tid, 5, _XfCharString_get_data_pdesc, _XfCharString_get_data_pfunc
};
void CharString::get_data(Data& d) {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &d;
    _b.invoke(this, _XfCharString_get_data_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfCharString_get_char_data_pdesc[3] = { 2, 4, 3 };
MarshalBuffer::ArgMarshal _XfCharString_get_char_data_pfunc[] = {
    &_XfCharString_CharData_put, &_XfCharString_CharData_get,

};
MarshalBuffer::ArgInfo _XfCharString_get_char_data_pinfo = {
    &_XfCharString_tid, 6, _XfCharString_get_char_data_pdesc, _XfCharString_get_char_data_pfunc
};
void CharString::get_char_data(CharData& d) {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &d;
    _b.invoke(this, _XfCharString_get_char_data_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfCharString_put_data_pdesc[3] = { 2, 4, 1 };
MarshalBuffer::ArgMarshal _XfCharString_put_data_pfunc[] = {
    &_XfCharString_Data_put, &_XfCharString_Data_get,

};
MarshalBuffer::ArgInfo _XfCharString_put_data_pinfo = {
    &_XfCharString_tid, 7, _XfCharString_put_data_pdesc, _XfCharString_put_data_pfunc
};
void CharString::put_data(const Data& d) {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &d;
    _b.invoke(this, _XfCharString_put_data_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfCharString_put_char_data_pdesc[3] = { 2, 4, 1 };
MarshalBuffer::ArgMarshal _XfCharString_put_char_data_pfunc[] = {
    &_XfCharString_CharData_put, &_XfCharString_CharData_get,

};
MarshalBuffer::ArgInfo _XfCharString_put_char_data_pinfo = {
    &_XfCharString_tid, 8, _XfCharString_put_char_data_pdesc, _XfCharString_put_char_data_pfunc
};
void CharString::put_char_data(const CharData& d) {
    MarshalBuffer _b;
    extern TypeObjId _XfCharString_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &d;
    _b.invoke(this, _XfCharString_put_char_data_pinfo, _arg);
}
void _XfCharString_Data_put(MarshalBuffer& _b, const CharString::Data& _this) {
    _b.put_seq(&_this, sizeof(CharCode));
}
void _XfCharString_Data_get(MarshalBuffer& _b, CharString::Data& _this) {
    _b.get_seq(&_this, sizeof(CharCode));
}
void _XfCharString_CharData_put(MarshalBuffer& _b, const CharString::CharData& _this) {
    _b.put_seq(&_this, sizeof(Char));
}
void _XfCharString_CharData_get(MarshalBuffer& _b, CharString::CharData& _this) {
    _b.get_seq(&_this, sizeof(Char));
}
//+

//+ FrescoObject::%init,type+dii,client
FrescoObject::FrescoObject() { }
FrescoObject::~FrescoObject() { }
void* FrescoObject::_this() { return this; }

extern TypeObj_Descriptor _XfLong_type, _XfTag_type, _XfFrescoObject_type;

TypeObj_OpData _XfFrescoObject_methods[] = {
    { "ref__", &_XfLong_type, 1 },
    { "attach", &_XfTag_type, 1 },
    { "detach", &_Xfvoid_type, 1 },
    { "disconnect", &_Xfvoid_type, 0 },
    { "notify_observers", &_Xfvoid_type, 0 },
    { "update", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfFrescoObject_params[] = {
    /* ref__ */
        { "references", 0, &_XfLong_type },
    /* attach */
        { "observer", 0, &_XfFrescoObject_type },
    /* detach */
        { "attach_tag", 0, &_XfTag_type }
};
extern TypeObjId _XfFrescoObject_tid;
extern void _XfFrescoObject_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfFrescoObject_type = {
    /* type */ 0,
    /* id */ &_XfFrescoObject_tid,
    "FrescoObject",
    /* parents */ nil, /* offsets */ nil, /* excepts */ nil,
    _XfFrescoObject_methods, _XfFrescoObject_params,
    &_XfFrescoObject_receive
};

FrescoObjectRef FrescoObject::_narrow(BaseObjectRef o) {
    return (FrescoObjectRef)_BaseObject_tnarrow(
        o, _XfFrescoObject_tid, &_XfFrescoObjectStub_create
    );
}
TypeObjId FrescoObject::_tid() { return _XfFrescoObject_tid; }
void _XfFrescoObject_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfFrescoObject_tid;
    FrescoObjectRef _this = (FrescoObjectRef)_BaseObject_tcast(_object, _XfFrescoObject_tid);
    switch (_m) {
        case /* ref__ */ 0: {
            extern MarshalBuffer::ArgInfo _XfFrescoObject_ref___pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Long references;
            _arg[1].u_addr = &references;
            _b.receive(_XfFrescoObject_ref___pinfo, _arg);
            _arg[0].u_long = _this->ref__(references);
            _b.reply(_XfFrescoObject_ref___pinfo, _arg);
            break;
        }
        case /* attach */ 1: {
            extern MarshalBuffer::ArgInfo _XfFrescoObject_attach_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            FrescoObjectRef observer;
            _arg[1].u_addr = &observer;
            _b.receive(_XfFrescoObject_attach_pinfo, _arg);
            _arg[0].u_unsigned_long = _this->attach(observer);
            _b.reply(_XfFrescoObject_attach_pinfo, _arg);
            break;
        }
        case /* detach */ 2: {
            extern MarshalBuffer::ArgInfo _XfFrescoObject_detach_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Tag attach_tag;
            _arg[1].u_addr = &attach_tag;
            _b.receive(_XfFrescoObject_detach_pinfo, _arg);
            _this->detach(attach_tag);
            _b.reply(_XfFrescoObject_detach_pinfo, _arg);
            break;
        }
        case /* disconnect */ 3: {
            extern MarshalBuffer::ArgInfo _XfFrescoObject_disconnect_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->disconnect();
            _b.reply(_XfFrescoObject_disconnect_pinfo, _arg);
            break;
        }
        case /* notify_observers */ 4: {
            extern MarshalBuffer::ArgInfo _XfFrescoObject_notify_observers_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->notify_observers();
            _b.reply(_XfFrescoObject_notify_observers_pinfo, _arg);
            break;
        }
        case /* update */ 5: {
            extern MarshalBuffer::ArgInfo _XfFrescoObject_update_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->update();
            _b.reply(_XfFrescoObject_update_pinfo, _arg);
            break;
        }
    }
}
FrescoObjectStub::FrescoObjectStub(Exchange* e) { exch_ = e; }
FrescoObjectStub::~FrescoObjectStub() { }
BaseObjectRef _XfFrescoObjectStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new FrescoObjectStub(e);
}
Exchange* FrescoObjectStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfFrescoObject_ref___pdesc[3] = { 2, 32, 33 };
MarshalBuffer::ArgInfo _XfFrescoObject_ref___pinfo = {
    &_XfFrescoObject_tid, 0, _XfFrescoObject_ref___pdesc, 0
};
Long FrescoObject::ref__(Long references) {
    MarshalBuffer _b;
    extern TypeObjId _XfFrescoObject_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = references;
    _b.invoke(this, _XfFrescoObject_ref___pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfFrescoObject_attach_pdesc[3] = { 2, 36, 61 };
MarshalBuffer::ArgMarshal _XfFrescoObject_attach_pfunc[] = {
    &_XfFrescoObjectStub_create,

};
MarshalBuffer::ArgInfo _XfFrescoObject_attach_pinfo = {
    &_XfFrescoObject_tid, 1, _XfFrescoObject_attach_pdesc, _XfFrescoObject_attach_pfunc
};
Tag FrescoObject::attach(FrescoObject_in observer) {
    MarshalBuffer _b;
    extern TypeObjId _XfFrescoObject_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = observer;
    _b.invoke(this, _XfFrescoObject_attach_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfFrescoObject_detach_pdesc[3] = { 2, 4, 37 };
MarshalBuffer::ArgInfo _XfFrescoObject_detach_pinfo = {
    &_XfFrescoObject_tid, 2, _XfFrescoObject_detach_pdesc, 0
};
void FrescoObject::detach(Tag attach_tag) {
    MarshalBuffer _b;
    extern TypeObjId _XfFrescoObject_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_unsigned_long = attach_tag;
    _b.invoke(this, _XfFrescoObject_detach_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFrescoObject_disconnect_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfFrescoObject_disconnect_pinfo = {
    &_XfFrescoObject_tid, 3, _XfFrescoObject_disconnect_pdesc, 0
};
void FrescoObject::disconnect() {
    MarshalBuffer _b;
    extern TypeObjId _XfFrescoObject_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFrescoObject_disconnect_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFrescoObject_notify_observers_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfFrescoObject_notify_observers_pinfo = {
    &_XfFrescoObject_tid, 4, _XfFrescoObject_notify_observers_pdesc, 0
};
void FrescoObject::notify_observers() {
    MarshalBuffer _b;
    extern TypeObjId _XfFrescoObject_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFrescoObject_notify_observers_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFrescoObject_update_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfFrescoObject_update_pinfo = {
    &_XfFrescoObject_tid, 5, _XfFrescoObject_update_pdesc, 0
};
void FrescoObject::update() {
    MarshalBuffer _b;
    extern TypeObjId _XfFrescoObject_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfFrescoObject_update_pinfo, _arg);
}
//+

//+ Region::%init,type+dii,client
Region::Region() { }
Region::~Region() { }
void* Region::_this() { return this; }

extern TypeObj_Descriptor _XfVertex_type, _XfAxis_type, _XfRegion_type, 
    _XfTransform_type, _XfRegion_BoundingSpan_type;

TypeObj_OpData _XfRegion_methods[] = {
    { "contains", &_XfBoolean_type, 1 },
    { "contains_plane", &_XfBoolean_type, 2 },
    { "intersects", &_XfBoolean_type, 1 },
    { "copy", &_Xfvoid_type, 1 },
    { "merge_intersect", &_Xfvoid_type, 1 },
    { "merge_union", &_Xfvoid_type, 1 },
    { "subtract", &_Xfvoid_type, 1 },
    { "apply_transform", &_Xfvoid_type, 1 },
    { "bounds", &_Xfvoid_type, 2 },
    { "origin", &_Xfvoid_type, 1 },
    { "span", &_Xfvoid_type, 2 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfRegion_params[] = {
    /* contains */
        { "v", 0, &_XfVertex_type },
    /* contains_plane */
        { "v", 0, &_XfVertex_type },
        { "a", 0, &_XfAxis_type },
    /* intersects */
        { "r", 0, &_XfRegion_type },
    /* copy */
        { "r", 0, &_XfRegion_type },
    /* merge_intersect */
        { "r", 0, &_XfRegion_type },
    /* merge_union */
        { "r", 0, &_XfRegion_type },
    /* subtract */
        { "r", 0, &_XfRegion_type },
    /* apply_transform */
        { "t", 0, &_XfTransform_type },
    /* bounds */
        { "lower", 1, &_XfVertex_type },
        { "upper", 1, &_XfVertex_type },
    /* origin */
        { "v", 1, &_XfVertex_type },
    /* span */
        { "a", 0, &_XfAxis_type },
        { "s", 1, &_XfRegion_BoundingSpan_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfRegion_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfRegion_tid;
extern void _XfRegion_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfRegion_type = {
    /* type */ 0,
    /* id */ &_XfRegion_tid,
    "Region",
    _XfRegion_parents, /* offsets */ nil, /* excepts */ nil,
    _XfRegion_methods, _XfRegion_params,
    &_XfRegion_receive
};

RegionRef Region::_narrow(BaseObjectRef o) {
    return (RegionRef)_BaseObject_tnarrow(
        o, _XfRegion_tid, &_XfRegionStub_create
    );
}
TypeObjId Region::_tid() { return _XfRegion_tid; }
void _XfRegion_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfRegion_tid;
    RegionRef _this = (RegionRef)_BaseObject_tcast(_object, _XfRegion_tid);
    switch (_m) {
        case /* contains */ 0: {
            extern MarshalBuffer::ArgInfo _XfRegion_contains_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Vertex v;
            _arg[1].u_addr = &v;
            _b.receive(_XfRegion_contains_pinfo, _arg);
            _arg[0].u_boolean = _this->contains(v);
            _b.reply(_XfRegion_contains_pinfo, _arg);
            break;
        }
        case /* contains_plane */ 1: {
            extern MarshalBuffer::ArgInfo _XfRegion_contains_plane_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Vertex v;
            _arg[1].u_addr = &v;
            Axis a;
            _arg[2].u_addr = &a;
            _b.receive(_XfRegion_contains_plane_pinfo, _arg);
            _arg[0].u_boolean = _this->contains_plane(v, a);
            _b.reply(_XfRegion_contains_plane_pinfo, _arg);
            break;
        }
        case /* intersects */ 2: {
            extern MarshalBuffer::ArgInfo _XfRegion_intersects_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfRegion_intersects_pinfo, _arg);
            _arg[0].u_boolean = _this->intersects(r);
            _b.reply(_XfRegion_intersects_pinfo, _arg);
            break;
        }
        case /* copy */ 3: {
            extern MarshalBuffer::ArgInfo _XfRegion_copy_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfRegion_copy_pinfo, _arg);
            _this->copy(r);
            _b.reply(_XfRegion_copy_pinfo, _arg);
            break;
        }
        case /* merge_intersect */ 4: {
            extern MarshalBuffer::ArgInfo _XfRegion_merge_intersect_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfRegion_merge_intersect_pinfo, _arg);
            _this->merge_intersect(r);
            _b.reply(_XfRegion_merge_intersect_pinfo, _arg);
            break;
        }
        case /* merge_union */ 5: {
            extern MarshalBuffer::ArgInfo _XfRegion_merge_union_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfRegion_merge_union_pinfo, _arg);
            _this->merge_union(r);
            _b.reply(_XfRegion_merge_union_pinfo, _arg);
            break;
        }
        case /* subtract */ 6: {
            extern MarshalBuffer::ArgInfo _XfRegion_subtract_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            RegionRef r;
            _arg[1].u_addr = &r;
            _b.receive(_XfRegion_subtract_pinfo, _arg);
            _this->subtract(r);
            _b.reply(_XfRegion_subtract_pinfo, _arg);
            break;
        }
        case /* apply_transform */ 7: {
            extern MarshalBuffer::ArgInfo _XfRegion_apply_transform_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TransformRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfRegion_apply_transform_pinfo, _arg);
            _this->apply_transform(t);
            _b.reply(_XfRegion_apply_transform_pinfo, _arg);
            break;
        }
        case /* bounds */ 8: {
            extern MarshalBuffer::ArgInfo _XfRegion_bounds_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Vertex lower;
            _arg[1].u_addr = &lower;
            Vertex upper;
            _arg[2].u_addr = &upper;
            _b.receive(_XfRegion_bounds_pinfo, _arg);
            _this->bounds(lower, upper);
            _b.reply(_XfRegion_bounds_pinfo, _arg);
            break;
        }
        case /* origin */ 9: {
            extern MarshalBuffer::ArgInfo _XfRegion_origin_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Vertex v;
            _arg[1].u_addr = &v;
            _b.receive(_XfRegion_origin_pinfo, _arg);
            _this->origin(v);
            _b.reply(_XfRegion_origin_pinfo, _arg);
            break;
        }
        case /* span */ 10: {
            extern MarshalBuffer::ArgInfo _XfRegion_span_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Axis a;
            _arg[1].u_addr = &a;
            Region::BoundingSpan s;
            _arg[2].u_addr = &s;
            _b.receive(_XfRegion_span_pinfo, _arg);
            _this->span(a, s);
            _b.reply(_XfRegion_span_pinfo, _arg);
            break;
        }
    }
}
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfRegion_BoundingSpan_put(
    MarshalBuffer&, const Region::BoundingSpan&
);
extern void _XfRegion_BoundingSpan_get(
    MarshalBuffer&, Region::BoundingSpan&
);

RegionStub::RegionStub(Exchange* e) { exch_ = e; }
RegionStub::~RegionStub() { }
BaseObjectRef _XfRegionStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new RegionStub(e);
}
Exchange* RegionStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfRegion_contains_pdesc[3] = { 2, 12, 1 };
MarshalBuffer::ArgMarshal _XfRegion_contains_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfRegion_contains_pinfo = {
    &_XfRegion_tid, 0, _XfRegion_contains_pdesc, _XfRegion_contains_pfunc
};
Boolean Region::contains(const Vertex& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &v;
    _b.invoke(this, _XfRegion_contains_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfRegion_contains_plane_pdesc[4] = { 3, 12, 1, 33 };
MarshalBuffer::ArgMarshal _XfRegion_contains_plane_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfRegion_contains_plane_pinfo = {
    &_XfRegion_tid, 1, _XfRegion_contains_plane_pdesc, _XfRegion_contains_plane_pfunc
};
Boolean Region::contains_plane(const Vertex& v, Axis a) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_addr = &v;
    _arg[2].u_long = a;
    _b.invoke(this, _XfRegion_contains_plane_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfRegion_intersects_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfRegion_intersects_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfRegion_intersects_pinfo = {
    &_XfRegion_tid, 2, _XfRegion_intersects_pdesc, _XfRegion_intersects_pfunc
};
Boolean Region::intersects(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfRegion_intersects_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfRegion_copy_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfRegion_copy_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfRegion_copy_pinfo = {
    &_XfRegion_tid, 3, _XfRegion_copy_pdesc, _XfRegion_copy_pfunc
};
void Region::copy(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfRegion_copy_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRegion_merge_intersect_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfRegion_merge_intersect_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfRegion_merge_intersect_pinfo = {
    &_XfRegion_tid, 4, _XfRegion_merge_intersect_pdesc, _XfRegion_merge_intersect_pfunc
};
void Region::merge_intersect(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfRegion_merge_intersect_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRegion_merge_union_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfRegion_merge_union_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfRegion_merge_union_pinfo = {
    &_XfRegion_tid, 5, _XfRegion_merge_union_pdesc, _XfRegion_merge_union_pfunc
};
void Region::merge_union(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfRegion_merge_union_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRegion_subtract_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfRegion_subtract_pfunc[] = {
    &_XfRegionStub_create,

};
MarshalBuffer::ArgInfo _XfRegion_subtract_pinfo = {
    &_XfRegion_tid, 6, _XfRegion_subtract_pdesc, _XfRegion_subtract_pfunc
};
void Region::subtract(Region_in r) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = r;
    _b.invoke(this, _XfRegion_subtract_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRegion_apply_transform_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfRegion_apply_transform_pfunc[] = {
    &_XfTransformStub_create,

};
MarshalBuffer::ArgInfo _XfRegion_apply_transform_pinfo = {
    &_XfRegion_tid, 7, _XfRegion_apply_transform_pdesc, _XfRegion_apply_transform_pfunc
};
void Region::apply_transform(Transform_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfRegion_apply_transform_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRegion_bounds_pdesc[4] = { 3, 4, 2, 2 };
MarshalBuffer::ArgMarshal _XfRegion_bounds_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfRegion_bounds_pinfo = {
    &_XfRegion_tid, 8, _XfRegion_bounds_pdesc, _XfRegion_bounds_pfunc
};
void Region::bounds(Vertex& lower, Vertex& upper) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_addr = &lower;
    _arg[2].u_addr = &upper;
    _b.invoke(this, _XfRegion_bounds_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRegion_origin_pdesc[3] = { 2, 4, 2 };
MarshalBuffer::ArgMarshal _XfRegion_origin_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfRegion_origin_pinfo = {
    &_XfRegion_tid, 9, _XfRegion_origin_pdesc, _XfRegion_origin_pfunc
};
void Region::origin(Vertex& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &v;
    _b.invoke(this, _XfRegion_origin_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfRegion_span_pdesc[4] = { 3, 4, 33, 2 };
MarshalBuffer::ArgMarshal _XfRegion_span_pfunc[] = {
    &_XfRegion_BoundingSpan_put, &_XfRegion_BoundingSpan_get,

};
MarshalBuffer::ArgInfo _XfRegion_span_pinfo = {
    &_XfRegion_tid, 10, _XfRegion_span_pdesc, _XfRegion_span_pfunc
};
void Region::span(Axis a, Region::BoundingSpan& s) {
    MarshalBuffer _b;
    extern TypeObjId _XfRegion_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_long = a;
    _arg[2].u_addr = &s;
    _b.invoke(this, _XfRegion_span_pinfo, _arg);
}
void _XfRegion_BoundingSpan_put(MarshalBuffer& _b, const Region::BoundingSpan& _this) {
    _b.put_float(_this.begin);
    _b.put_float(_this.end);
    _b.put_float(_this.origin);
    _b.put_float(_this.length);
    _b.put_float(_this.align);
}
void _XfRegion_BoundingSpan_get(MarshalBuffer& _b, Region::BoundingSpan& _this) {
    _this.begin = _b.get_float();
    _this.end = _b.get_float();
    _this.origin = _b.get_float();
    _this.length = _b.get_float();
    _this.align = _b.get_float();
}
//+

//+ Style::%init,type+dii,client
Style::Style() { }
Style::~Style() { }
void* Style::_this() { return this; }

extern TypeObj_Descriptor _XfStyle_type, _XfCharString_type, _XfStyleValue_type, 
    _XfStyleVisitor_type;

TypeObj_OpData _XfStyle_methods[] = {
    { "new_style", &_XfStyle_type, 0 },
    { "parent_style", &_XfStyle_type, 0 },
    { "link_parent", &_Xfvoid_type, 1 },
    { "unlink_parent", &_Xfvoid_type, 0 },
    { "link_child", &_XfTag_type, 1 },
    { "unlink_child", &_Xfvoid_type, 1 },
    { "merge", &_Xfvoid_type, 1 },
    { "_get_name", &_XfCharString_type, 0 },
    { "_set_name", &_Xfvoid_type, 1 },
    { "alias", &_Xfvoid_type, 1 },
    { "is_on", &_XfBoolean_type, 1 },
    { "bind", &_XfStyleValue_type, 1 },
    { "unbind", &_Xfvoid_type, 1 },
    { "resolve", &_XfStyleValue_type, 1 },
    { "resolve_wildcard", &_XfStyleValue_type, 2 },
    { "match", &_XfLong_type, 1 },
    { "visit_aliases", &_Xfvoid_type, 1 },
    { "visit_attributes", &_Xfvoid_type, 1 },
    { "visit_styles", &_Xfvoid_type, 1 },
    { "lock", &_Xfvoid_type, 0 },
    { "unlock", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfStyle_params[] = {
    /* link_parent */
        { "parent", 0, &_XfStyle_type },
    /* link_child */
        { "child", 0, &_XfStyle_type },
    /* unlink_child */
        { "link_tag", 0, &_XfTag_type },
    /* merge */
        { "s", 0, &_XfStyle_type },
    /* name */
        { "_p", 0, &_XfCharString_type },
    /* alias */
        { "s", 0, &_XfCharString_type },
    /* is_on */
        { "name", 0, &_XfCharString_type },
    /* bind */
        { "name", 0, &_XfCharString_type },
    /* unbind */
        { "name", 0, &_XfCharString_type },
    /* resolve */
        { "name", 0, &_XfCharString_type },
    /* resolve_wildcard */
        { "name", 0, &_XfCharString_type },
        { "start", 0, &_XfStyle_type },
    /* match */
        { "name", 0, &_XfCharString_type },
    /* visit_aliases */
        { "v", 0, &_XfStyleVisitor_type },
    /* visit_attributes */
        { "v", 0, &_XfStyleVisitor_type },
    /* visit_styles */
        { "v", 0, &_XfStyleVisitor_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfStyle_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfStyle_tid;
extern void _XfStyle_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfStyle_type = {
    /* type */ 0,
    /* id */ &_XfStyle_tid,
    "Style",
    _XfStyle_parents, /* offsets */ nil, /* excepts */ nil,
    _XfStyle_methods, _XfStyle_params,
    &_XfStyle_receive
};

StyleRef Style::_narrow(BaseObjectRef o) {
    return (StyleRef)_BaseObject_tnarrow(
        o, _XfStyle_tid, &_XfStyleStub_create
    );
}
TypeObjId Style::_tid() { return _XfStyle_tid; }
void _XfStyle_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfStyle_tid;
    StyleRef _this = (StyleRef)_BaseObject_tcast(_object, _XfStyle_tid);
    switch (_m) {
        case /* new_style */ 0: {
            extern MarshalBuffer::ArgInfo _XfStyle_new_style_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->new_style();
            _b.reply(_XfStyle_new_style_pinfo, _arg);
            break;
        }
        case /* parent_style */ 1: {
            extern MarshalBuffer::ArgInfo _XfStyle_parent_style_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->parent_style();
            _b.reply(_XfStyle_parent_style_pinfo, _arg);
            break;
        }
        case /* link_parent */ 2: {
            extern MarshalBuffer::ArgInfo _XfStyle_link_parent_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef parent;
            _arg[1].u_addr = &parent;
            _b.receive(_XfStyle_link_parent_pinfo, _arg);
            _this->link_parent(parent);
            _b.reply(_XfStyle_link_parent_pinfo, _arg);
            break;
        }
        case /* unlink_parent */ 3: {
            extern MarshalBuffer::ArgInfo _XfStyle_unlink_parent_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->unlink_parent();
            _b.reply(_XfStyle_unlink_parent_pinfo, _arg);
            break;
        }
        case /* link_child */ 4: {
            extern MarshalBuffer::ArgInfo _XfStyle_link_child_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef child;
            _arg[1].u_addr = &child;
            _b.receive(_XfStyle_link_child_pinfo, _arg);
            _arg[0].u_unsigned_long = _this->link_child(child);
            _b.reply(_XfStyle_link_child_pinfo, _arg);
            break;
        }
        case /* unlink_child */ 5: {
            extern MarshalBuffer::ArgInfo _XfStyle_unlink_child_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Tag link_tag;
            _arg[1].u_addr = &link_tag;
            _b.receive(_XfStyle_unlink_child_pinfo, _arg);
            _this->unlink_child(link_tag);
            _b.reply(_XfStyle_unlink_child_pinfo, _arg);
            break;
        }
        case /* merge */ 6: {
            extern MarshalBuffer::ArgInfo _XfStyle_merge_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfStyle_merge_pinfo, _arg);
            _this->merge(s);
            _b.reply(_XfStyle_merge_pinfo, _arg);
            break;
        }
        case /* _get_name */ 7: {
            extern MarshalBuffer::ArgInfo _XfStyle__get_name_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->name();
            _b.reply(_XfStyle__get_name_pinfo, _arg);
            break;
        }
        case /* _set_name */ 8: {
            extern MarshalBuffer::ArgInfo _XfStyle__set_name_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfStyle__set_name_pinfo, _arg);
            _this->name(_p);
            _b.reply(_XfStyle__set_name_pinfo, _arg);
            break;
        }
        case /* alias */ 9: {
            extern MarshalBuffer::ArgInfo _XfStyle_alias_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfStyle_alias_pinfo, _arg);
            _this->alias(s);
            _b.reply(_XfStyle_alias_pinfo, _arg);
            break;
        }
        case /* is_on */ 10: {
            extern MarshalBuffer::ArgInfo _XfStyle_is_on_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef name;
            _arg[1].u_addr = &name;
            _b.receive(_XfStyle_is_on_pinfo, _arg);
            _arg[0].u_boolean = _this->is_on(name);
            _b.reply(_XfStyle_is_on_pinfo, _arg);
            break;
        }
        case /* bind */ 11: {
            extern MarshalBuffer::ArgInfo _XfStyle_bind_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef name;
            _arg[1].u_addr = &name;
            _b.receive(_XfStyle_bind_pinfo, _arg);
            _arg[0].u_objref = _this->bind(name);
            _b.reply(_XfStyle_bind_pinfo, _arg);
            break;
        }
        case /* unbind */ 12: {
            extern MarshalBuffer::ArgInfo _XfStyle_unbind_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef name;
            _arg[1].u_addr = &name;
            _b.receive(_XfStyle_unbind_pinfo, _arg);
            _this->unbind(name);
            _b.reply(_XfStyle_unbind_pinfo, _arg);
            break;
        }
        case /* resolve */ 13: {
            extern MarshalBuffer::ArgInfo _XfStyle_resolve_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef name;
            _arg[1].u_addr = &name;
            _b.receive(_XfStyle_resolve_pinfo, _arg);
            _arg[0].u_objref = _this->resolve(name);
            _b.reply(_XfStyle_resolve_pinfo, _arg);
            break;
        }
        case /* resolve_wildcard */ 14: {
            extern MarshalBuffer::ArgInfo _XfStyle_resolve_wildcard_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            CharStringRef name;
            _arg[1].u_addr = &name;
            StyleRef start;
            _arg[2].u_addr = &start;
            _b.receive(_XfStyle_resolve_wildcard_pinfo, _arg);
            _arg[0].u_objref = _this->resolve_wildcard(name, start);
            _b.reply(_XfStyle_resolve_wildcard_pinfo, _arg);
            break;
        }
        case /* match */ 15: {
            extern MarshalBuffer::ArgInfo _XfStyle_match_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef name;
            _arg[1].u_addr = &name;
            _b.receive(_XfStyle_match_pinfo, _arg);
            _arg[0].u_long = _this->match(name);
            _b.reply(_XfStyle_match_pinfo, _arg);
            break;
        }
        case /* visit_aliases */ 16: {
            extern MarshalBuffer::ArgInfo _XfStyle_visit_aliases_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleVisitorRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfStyle_visit_aliases_pinfo, _arg);
            _this->visit_aliases(v);
            _b.reply(_XfStyle_visit_aliases_pinfo, _arg);
            break;
        }
        case /* visit_attributes */ 17: {
            extern MarshalBuffer::ArgInfo _XfStyle_visit_attributes_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleVisitorRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfStyle_visit_attributes_pinfo, _arg);
            _this->visit_attributes(v);
            _b.reply(_XfStyle_visit_attributes_pinfo, _arg);
            break;
        }
        case /* visit_styles */ 18: {
            extern MarshalBuffer::ArgInfo _XfStyle_visit_styles_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleVisitorRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfStyle_visit_styles_pinfo, _arg);
            _this->visit_styles(v);
            _b.reply(_XfStyle_visit_styles_pinfo, _arg);
            break;
        }
        case /* lock */ 19: {
            extern MarshalBuffer::ArgInfo _XfStyle_lock_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->lock();
            _b.reply(_XfStyle_lock_pinfo, _arg);
            break;
        }
        case /* unlock */ 20: {
            extern MarshalBuffer::ArgInfo _XfStyle_unlock_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->unlock();
            _b.reply(_XfStyle_unlock_pinfo, _arg);
            break;
        }
    }
}
StyleStub::StyleStub(Exchange* e) { exch_ = e; }
StyleStub::~StyleStub() { }
BaseObjectRef _XfStyleStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new StyleStub(e);
}
Exchange* StyleStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfStyle_new_style_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfStyle_new_style_pfunc[] = {
    &_XfStyleStub_create
};
MarshalBuffer::ArgInfo _XfStyle_new_style_pinfo = {
    &_XfStyle_tid, 0, _XfStyle_new_style_pdesc, _XfStyle_new_style_pfunc
};
StyleRef Style::new_style() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyle_new_style_pinfo, _arg);
    return (StyleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfStyle_parent_style_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfStyle_parent_style_pfunc[] = {
    &_XfStyleStub_create
};
MarshalBuffer::ArgInfo _XfStyle_parent_style_pinfo = {
    &_XfStyle_tid, 1, _XfStyle_parent_style_pdesc, _XfStyle_parent_style_pfunc
};
StyleRef Style::parent_style() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyle_parent_style_pinfo, _arg);
    return (StyleRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfStyle_link_parent_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyle_link_parent_pfunc[] = {
    &_XfStyleStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_link_parent_pinfo = {
    &_XfStyle_tid, 2, _XfStyle_link_parent_pdesc, _XfStyle_link_parent_pfunc
};
void Style::link_parent(Style_in parent) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = parent;
    _b.invoke(this, _XfStyle_link_parent_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_unlink_parent_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfStyle_unlink_parent_pinfo = {
    &_XfStyle_tid, 3, _XfStyle_unlink_parent_pdesc, 0
};
void Style::unlink_parent() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyle_unlink_parent_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_link_child_pdesc[3] = { 2, 36, 61 };
MarshalBuffer::ArgMarshal _XfStyle_link_child_pfunc[] = {
    &_XfStyleStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_link_child_pinfo = {
    &_XfStyle_tid, 4, _XfStyle_link_child_pdesc, _XfStyle_link_child_pfunc
};
Tag Style::link_child(Style_in child) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = child;
    _b.invoke(this, _XfStyle_link_child_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfStyle_unlink_child_pdesc[3] = { 2, 4, 37 };
MarshalBuffer::ArgInfo _XfStyle_unlink_child_pinfo = {
    &_XfStyle_tid, 5, _XfStyle_unlink_child_pdesc, 0
};
void Style::unlink_child(Tag link_tag) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_unsigned_long = link_tag;
    _b.invoke(this, _XfStyle_unlink_child_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_merge_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyle_merge_pfunc[] = {
    &_XfStyleStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_merge_pinfo = {
    &_XfStyle_tid, 6, _XfStyle_merge_pdesc, _XfStyle_merge_pfunc
};
void Style::merge(Style_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfStyle_merge_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle__get_name_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfStyle__get_name_pfunc[] = {
    &_XfCharStringStub_create
};
MarshalBuffer::ArgInfo _XfStyle__get_name_pinfo = {
    &_XfStyle_tid, 7, _XfStyle__get_name_pdesc, _XfStyle__get_name_pfunc
};
CharStringRef Style::name() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyle__get_name_pinfo, _arg);
    return (CharStringRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfStyle__set_name_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyle__set_name_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfStyle__set_name_pinfo = {
    &_XfStyle_tid, 8, _XfStyle__set_name_pdesc, _XfStyle__set_name_pfunc
};
void Style::name(CharString_in _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = _p;
    _b.invoke(this, _XfStyle__set_name_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_alias_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyle_alias_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_alias_pinfo = {
    &_XfStyle_tid, 9, _XfStyle_alias_pdesc, _XfStyle_alias_pfunc
};
void Style::alias(CharString_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfStyle_alias_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_is_on_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfStyle_is_on_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_is_on_pinfo = {
    &_XfStyle_tid, 10, _XfStyle_is_on_pdesc, _XfStyle_is_on_pfunc
};
Boolean Style::is_on(CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = name;
    _b.invoke(this, _XfStyle_is_on_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyle_bind_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfStyle_bind_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfStyleValueStub_create
};
MarshalBuffer::ArgInfo _XfStyle_bind_pinfo = {
    &_XfStyle_tid, 11, _XfStyle_bind_pdesc, _XfStyle_bind_pfunc
};
StyleValueRef Style::bind(CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = name;
    _b.invoke(this, _XfStyle_bind_pinfo, _arg);
    return (StyleValueRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfStyle_unbind_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyle_unbind_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_unbind_pinfo = {
    &_XfStyle_tid, 12, _XfStyle_unbind_pdesc, _XfStyle_unbind_pfunc
};
void Style::unbind(CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = name;
    _b.invoke(this, _XfStyle_unbind_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_resolve_pdesc[3] = { 2, 60, 61 };
MarshalBuffer::ArgMarshal _XfStyle_resolve_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfStyleValueStub_create
};
MarshalBuffer::ArgInfo _XfStyle_resolve_pinfo = {
    &_XfStyle_tid, 13, _XfStyle_resolve_pdesc, _XfStyle_resolve_pfunc
};
StyleValueRef Style::resolve(CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = name;
    _b.invoke(this, _XfStyle_resolve_pinfo, _arg);
    return (StyleValueRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfStyle_resolve_wildcard_pdesc[4] = { 3, 60, 61, 61 };
MarshalBuffer::ArgMarshal _XfStyle_resolve_wildcard_pfunc[] = {
    &_XfCharStringStub_create,
    &_XfStyleStub_create,
    &_XfStyleValueStub_create
};
MarshalBuffer::ArgInfo _XfStyle_resolve_wildcard_pinfo = {
    &_XfStyle_tid, 14, _XfStyle_resolve_wildcard_pdesc, _XfStyle_resolve_wildcard_pfunc
};
StyleValueRef Style::resolve_wildcard(CharString_in name, Style_in start) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = name;
    _arg[2].u_objref = start;
    _b.invoke(this, _XfStyle_resolve_wildcard_pinfo, _arg);
    return (StyleValueRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfStyle_match_pdesc[3] = { 2, 32, 61 };
MarshalBuffer::ArgMarshal _XfStyle_match_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_match_pinfo = {
    &_XfStyle_tid, 15, _XfStyle_match_pdesc, _XfStyle_match_pfunc
};
Long Style::match(CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = name;
    _b.invoke(this, _XfStyle_match_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfStyle_visit_aliases_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyle_visit_aliases_pfunc[] = {
    &_XfStyleVisitorStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_visit_aliases_pinfo = {
    &_XfStyle_tid, 16, _XfStyle_visit_aliases_pdesc, _XfStyle_visit_aliases_pfunc
};
void Style::visit_aliases(StyleVisitor_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfStyle_visit_aliases_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_visit_attributes_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyle_visit_attributes_pfunc[] = {
    &_XfStyleVisitorStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_visit_attributes_pinfo = {
    &_XfStyle_tid, 17, _XfStyle_visit_attributes_pdesc, _XfStyle_visit_attributes_pfunc
};
void Style::visit_attributes(StyleVisitor_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfStyle_visit_attributes_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_visit_styles_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyle_visit_styles_pfunc[] = {
    &_XfStyleVisitorStub_create,

};
MarshalBuffer::ArgInfo _XfStyle_visit_styles_pinfo = {
    &_XfStyle_tid, 18, _XfStyle_visit_styles_pdesc, _XfStyle_visit_styles_pfunc
};
void Style::visit_styles(StyleVisitor_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfStyle_visit_styles_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_lock_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfStyle_lock_pinfo = {
    &_XfStyle_tid, 19, _XfStyle_lock_pdesc, 0
};
void Style::lock() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyle_lock_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyle_unlock_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfStyle_unlock_pinfo = {
    &_XfStyle_tid, 20, _XfStyle_unlock_pdesc, 0
};
void Style::unlock() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyle_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyle_unlock_pinfo, _arg);
}
//+

//+ StyleValue::%init,type+dii,client
StyleValue::StyleValue() { }
StyleValue::~StyleValue() { }
void* StyleValue::_this() { return this; }

extern TypeObj_Descriptor _XfCoord_type, _XfDouble_type;

TypeObj_OpData _XfStyleValue_methods[] = {
    { "name", &_XfCharString_type, 0 },
    { "uninitialized", &_XfBoolean_type, 0 },
    { "_get_priority", &_XfLong_type, 0 },
    { "_set_priority", &_Xfvoid_type, 1 },
    { "is_on", &_XfBoolean_type, 0 },
    { "read_boolean", &_XfBoolean_type, 1 },
    { "write_boolean", &_Xfvoid_type, 1 },
    { "read_coord", &_XfBoolean_type, 1 },
    { "write_coord", &_Xfvoid_type, 1 },
    { "read_integer", &_XfBoolean_type, 1 },
    { "write_integer", &_Xfvoid_type, 1 },
    { "read_real", &_XfBoolean_type, 1 },
    { "write_real", &_Xfvoid_type, 1 },
    { "read_string", &_XfBoolean_type, 1 },
    { "write_string", &_Xfvoid_type, 1 },
    { "read_value", &_XfBoolean_type, 1 },
    { "write_value", &_Xfvoid_type, 1 },
    { "lock", &_Xfvoid_type, 0 },
    { "unlock", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfStyleValue_params[] = {
    /* priority */
        { "_p", 0, &_XfLong_type },
    /* read_boolean */
        { "b", 1, &_XfBoolean_type },
    /* write_boolean */
        { "b", 0, &_XfBoolean_type },
    /* read_coord */
        { "c", 1, &_XfCoord_type },
    /* write_coord */
        { "c", 0, &_XfCoord_type },
    /* read_integer */
        { "i", 1, &_XfLong_type },
    /* write_integer */
        { "i", 0, &_XfLong_type },
    /* read_real */
        { "d", 1, &_XfDouble_type },
    /* write_real */
        { "d", 0, &_XfDouble_type },
    /* read_string */
        { "s", 1, &_XfCharString_type },
    /* write_string */
        { "s", 0, &_XfCharString_type },
    /* read_value */
        { "s", 1, &_XfFrescoObject_type },
    /* write_value */
        { "s", 0, &_XfFrescoObject_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfStyleValue_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfStyleValue_tid;
extern void _XfStyleValue_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfStyleValue_type = {
    /* type */ 0,
    /* id */ &_XfStyleValue_tid,
    "StyleValue",
    _XfStyleValue_parents, /* offsets */ nil, /* excepts */ nil,
    _XfStyleValue_methods, _XfStyleValue_params,
    &_XfStyleValue_receive
};

StyleValueRef StyleValue::_narrow(BaseObjectRef o) {
    return (StyleValueRef)_BaseObject_tnarrow(
        o, _XfStyleValue_tid, &_XfStyleValueStub_create
    );
}
TypeObjId StyleValue::_tid() { return _XfStyleValue_tid; }
void _XfStyleValue_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfStyleValue_tid;
    StyleValueRef _this = (StyleValueRef)_BaseObject_tcast(_object, _XfStyleValue_tid);
    switch (_m) {
        case /* name */ 0: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_name_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->name();
            _b.reply(_XfStyleValue_name_pinfo, _arg);
            break;
        }
        case /* uninitialized */ 1: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_uninitialized_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->uninitialized();
            _b.reply(_XfStyleValue_uninitialized_pinfo, _arg);
            break;
        }
        case /* _get_priority */ 2: {
            extern MarshalBuffer::ArgInfo _XfStyleValue__get_priority_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->priority();
            _b.reply(_XfStyleValue__get_priority_pinfo, _arg);
            break;
        }
        case /* _set_priority */ 3: {
            extern MarshalBuffer::ArgInfo _XfStyleValue__set_priority_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Long _p;
            _arg[1].u_addr = &_p;
            _b.receive(_XfStyleValue__set_priority_pinfo, _arg);
            _this->priority(_p);
            _b.reply(_XfStyleValue__set_priority_pinfo, _arg);
            break;
        }
        case /* is_on */ 4: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_is_on_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->is_on();
            _b.reply(_XfStyleValue_is_on_pinfo, _arg);
            break;
        }
        case /* read_boolean */ 5: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_read_boolean_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Boolean b;
            _arg[1].u_addr = &b;
            _b.receive(_XfStyleValue_read_boolean_pinfo, _arg);
            _arg[0].u_boolean = _this->read_boolean(b);
            _b.reply(_XfStyleValue_read_boolean_pinfo, _arg);
            break;
        }
        case /* write_boolean */ 6: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_write_boolean_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Boolean b;
            _arg[1].u_addr = &b;
            _b.receive(_XfStyleValue_write_boolean_pinfo, _arg);
            _this->write_boolean(b);
            _b.reply(_XfStyleValue_write_boolean_pinfo, _arg);
            break;
        }
        case /* read_coord */ 7: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_read_coord_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord c;
            _arg[1].u_addr = &c;
            _b.receive(_XfStyleValue_read_coord_pinfo, _arg);
            _arg[0].u_boolean = _this->read_coord(c);
            _b.reply(_XfStyleValue_read_coord_pinfo, _arg);
            break;
        }
        case /* write_coord */ 8: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_write_coord_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Coord c;
            _arg[1].u_addr = &c;
            _b.receive(_XfStyleValue_write_coord_pinfo, _arg);
            _this->write_coord(c);
            _b.reply(_XfStyleValue_write_coord_pinfo, _arg);
            break;
        }
        case /* read_integer */ 9: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_read_integer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Long i;
            _arg[1].u_addr = &i;
            _b.receive(_XfStyleValue_read_integer_pinfo, _arg);
            _arg[0].u_boolean = _this->read_integer(i);
            _b.reply(_XfStyleValue_read_integer_pinfo, _arg);
            break;
        }
        case /* write_integer */ 10: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_write_integer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Long i;
            _arg[1].u_addr = &i;
            _b.receive(_XfStyleValue_write_integer_pinfo, _arg);
            _this->write_integer(i);
            _b.reply(_XfStyleValue_write_integer_pinfo, _arg);
            break;
        }
        case /* read_real */ 11: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_read_real_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Double d;
            _arg[1].u_addr = &d;
            _b.receive(_XfStyleValue_read_real_pinfo, _arg);
            _arg[0].u_boolean = _this->read_real(d);
            _b.reply(_XfStyleValue_read_real_pinfo, _arg);
            break;
        }
        case /* write_real */ 12: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_write_real_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Double d;
            _arg[1].u_addr = &d;
            _b.receive(_XfStyleValue_write_real_pinfo, _arg);
            _this->write_real(d);
            _b.reply(_XfStyleValue_write_real_pinfo, _arg);
            break;
        }
        case /* read_string */ 13: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_read_string_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfStyleValue_read_string_pinfo, _arg);
            _arg[0].u_boolean = _this->read_string(s);
            _b.reply(_XfStyleValue_read_string_pinfo, _arg);
            break;
        }
        case /* write_string */ 14: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_write_string_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfStyleValue_write_string_pinfo, _arg);
            _this->write_string(s);
            _b.reply(_XfStyleValue_write_string_pinfo, _arg);
            break;
        }
        case /* read_value */ 15: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_read_value_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            FrescoObjectRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfStyleValue_read_value_pinfo, _arg);
            _arg[0].u_boolean = _this->read_value(s);
            _b.reply(_XfStyleValue_read_value_pinfo, _arg);
            break;
        }
        case /* write_value */ 16: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_write_value_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            FrescoObjectRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfStyleValue_write_value_pinfo, _arg);
            _this->write_value(s);
            _b.reply(_XfStyleValue_write_value_pinfo, _arg);
            break;
        }
        case /* lock */ 17: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_lock_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->lock();
            _b.reply(_XfStyleValue_lock_pinfo, _arg);
            break;
        }
        case /* unlock */ 18: {
            extern MarshalBuffer::ArgInfo _XfStyleValue_unlock_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->unlock();
            _b.reply(_XfStyleValue_unlock_pinfo, _arg);
            break;
        }
    }
}
StyleValueStub::StyleValueStub(Exchange* e) { exch_ = e; }
StyleValueStub::~StyleValueStub() { }
BaseObjectRef _XfStyleValueStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new StyleValueStub(e);
}
Exchange* StyleValueStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfStyleValue_name_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfStyleValue_name_pfunc[] = {
    &_XfCharStringStub_create
};
MarshalBuffer::ArgInfo _XfStyleValue_name_pinfo = {
    &_XfStyleValue_tid, 0, _XfStyleValue_name_pdesc, _XfStyleValue_name_pfunc
};
CharStringRef StyleValue::name() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyleValue_name_pinfo, _arg);
    return (CharStringRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfStyleValue_uninitialized_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfStyleValue_uninitialized_pinfo = {
    &_XfStyleValue_tid, 1, _XfStyleValue_uninitialized_pdesc, 0
};
Boolean StyleValue::uninitialized() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyleValue_uninitialized_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleValue__get_priority_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfStyleValue__get_priority_pinfo = {
    &_XfStyleValue_tid, 2, _XfStyleValue__get_priority_pdesc, 0
};
Long StyleValue::priority() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyleValue__get_priority_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfStyleValue__set_priority_pdesc[3] = { 2, 4, 33 };
MarshalBuffer::ArgInfo _XfStyleValue__set_priority_pinfo = {
    &_XfStyleValue_tid, 3, _XfStyleValue__set_priority_pdesc, 0
};
void StyleValue::priority(Long _p) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = _p;
    _b.invoke(this, _XfStyleValue__set_priority_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyleValue_is_on_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfStyleValue_is_on_pinfo = {
    &_XfStyleValue_tid, 4, _XfStyleValue_is_on_pdesc, 0
};
Boolean StyleValue::is_on() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyleValue_is_on_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleValue_read_boolean_pdesc[3] = { 2, 12, 14 };
MarshalBuffer::ArgInfo _XfStyleValue_read_boolean_pinfo = {
    &_XfStyleValue_tid, 5, _XfStyleValue_read_boolean_pdesc, 0
};
Boolean StyleValue::read_boolean(Boolean& b) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &b;
    _b.invoke(this, _XfStyleValue_read_boolean_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleValue_write_boolean_pdesc[3] = { 2, 4, 13 };
MarshalBuffer::ArgInfo _XfStyleValue_write_boolean_pinfo = {
    &_XfStyleValue_tid, 6, _XfStyleValue_write_boolean_pdesc, 0
};
void StyleValue::write_boolean(Boolean b) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_boolean = b;
    _b.invoke(this, _XfStyleValue_write_boolean_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyleValue_read_coord_pdesc[3] = { 2, 12, 50 };
MarshalBuffer::ArgInfo _XfStyleValue_read_coord_pinfo = {
    &_XfStyleValue_tid, 7, _XfStyleValue_read_coord_pdesc, 0
};
Boolean StyleValue::read_coord(Coord& c) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &c;
    _b.invoke(this, _XfStyleValue_read_coord_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleValue_write_coord_pdesc[3] = { 2, 4, 49 };
MarshalBuffer::ArgInfo _XfStyleValue_write_coord_pinfo = {
    &_XfStyleValue_tid, 8, _XfStyleValue_write_coord_pdesc, 0
};
void StyleValue::write_coord(Coord c) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_float = c;
    _b.invoke(this, _XfStyleValue_write_coord_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyleValue_read_integer_pdesc[3] = { 2, 12, 34 };
MarshalBuffer::ArgInfo _XfStyleValue_read_integer_pinfo = {
    &_XfStyleValue_tid, 9, _XfStyleValue_read_integer_pdesc, 0
};
Boolean StyleValue::read_integer(Long& i) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &i;
    _b.invoke(this, _XfStyleValue_read_integer_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleValue_write_integer_pdesc[3] = { 2, 4, 33 };
MarshalBuffer::ArgInfo _XfStyleValue_write_integer_pinfo = {
    &_XfStyleValue_tid, 10, _XfStyleValue_write_integer_pdesc, 0
};
void StyleValue::write_integer(Long i) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = i;
    _b.invoke(this, _XfStyleValue_write_integer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyleValue_read_real_pdesc[3] = { 2, 12, 54 };
MarshalBuffer::ArgInfo _XfStyleValue_read_real_pinfo = {
    &_XfStyleValue_tid, 11, _XfStyleValue_read_real_pdesc, 0
};
Boolean StyleValue::read_real(Double& d) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &d;
    _b.invoke(this, _XfStyleValue_read_real_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleValue_write_real_pdesc[3] = { 2, 4, 53 };
MarshalBuffer::ArgInfo _XfStyleValue_write_real_pinfo = {
    &_XfStyleValue_tid, 12, _XfStyleValue_write_real_pdesc, 0
};
void StyleValue::write_real(Double d) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_double = d;
    _b.invoke(this, _XfStyleValue_write_real_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyleValue_read_string_pdesc[3] = { 2, 12, 62 };
MarshalBuffer::ArgMarshal _XfStyleValue_read_string_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfStyleValue_read_string_pinfo = {
    &_XfStyleValue_tid, 13, _XfStyleValue_read_string_pdesc, _XfStyleValue_read_string_pfunc
};
Boolean StyleValue::read_string(CharString_out& s) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &s;
    _b.invoke(this, _XfStyleValue_read_string_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleValue_write_string_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyleValue_write_string_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfStyleValue_write_string_pinfo = {
    &_XfStyleValue_tid, 14, _XfStyleValue_write_string_pdesc, _XfStyleValue_write_string_pfunc
};
void StyleValue::write_string(CharString_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfStyleValue_write_string_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyleValue_read_value_pdesc[3] = { 2, 12, 62 };
MarshalBuffer::ArgMarshal _XfStyleValue_read_value_pfunc[] = {
    &_XfFrescoObjectStub_create,

};
MarshalBuffer::ArgInfo _XfStyleValue_read_value_pinfo = {
    &_XfStyleValue_tid, 15, _XfStyleValue_read_value_pdesc, _XfStyleValue_read_value_pfunc
};
Boolean StyleValue::read_value(FrescoObject_out& s) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &s;
    _b.invoke(this, _XfStyleValue_read_value_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleValue_write_value_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfStyleValue_write_value_pfunc[] = {
    &_XfFrescoObjectStub_create,

};
MarshalBuffer::ArgInfo _XfStyleValue_write_value_pinfo = {
    &_XfStyleValue_tid, 16, _XfStyleValue_write_value_pdesc, _XfStyleValue_write_value_pfunc
};
void StyleValue::write_value(FrescoObject_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfStyleValue_write_value_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyleValue_lock_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfStyleValue_lock_pinfo = {
    &_XfStyleValue_tid, 17, _XfStyleValue_lock_pdesc, 0
};
void StyleValue::lock() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyleValue_lock_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfStyleValue_unlock_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfStyleValue_unlock_pinfo = {
    &_XfStyleValue_tid, 18, _XfStyleValue_unlock_pdesc, 0
};
void StyleValue::unlock() {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleValue_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfStyleValue_unlock_pinfo, _arg);
}
//+

//+ StyleVisitor::%init,type+dii,client
StyleVisitor::StyleVisitor() { }
StyleVisitor::~StyleVisitor() { }
void* StyleVisitor::_this() { return this; }

extern TypeObj_Descriptor _Xfvoid_type;

TypeObj_OpData _XfStyleVisitor_methods[] = {
    { "visit_alias", &_XfBoolean_type, 1 },
    { "visit_attribute", &_XfBoolean_type, 1 },
    { "visit_style", &_XfBoolean_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfStyleVisitor_params[] = {
    /* visit_alias */
        { "name", 0, &_XfCharString_type },
    /* visit_attribute */
        { "a", 0, &_XfStyleValue_type },
    /* visit_style */
        { "s", 0, &_XfStyle_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfStyleVisitor_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfStyleVisitor_tid;
extern void _XfStyleVisitor_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfStyleVisitor_type = {
    /* type */ 0,
    /* id */ &_XfStyleVisitor_tid,
    "StyleVisitor",
    _XfStyleVisitor_parents, /* offsets */ nil, /* excepts */ nil,
    _XfStyleVisitor_methods, _XfStyleVisitor_params,
    &_XfStyleVisitor_receive
};

StyleVisitorRef StyleVisitor::_narrow(BaseObjectRef o) {
    return (StyleVisitorRef)_BaseObject_tnarrow(
        o, _XfStyleVisitor_tid, &_XfStyleVisitorStub_create
    );
}
TypeObjId StyleVisitor::_tid() { return _XfStyleVisitor_tid; }
void _XfStyleVisitor_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfStyleVisitor_tid;
    StyleVisitorRef _this = (StyleVisitorRef)_BaseObject_tcast(_object, _XfStyleVisitor_tid);
    switch (_m) {
        case /* visit_alias */ 0: {
            extern MarshalBuffer::ArgInfo _XfStyleVisitor_visit_alias_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            CharStringRef name;
            _arg[1].u_addr = &name;
            _b.receive(_XfStyleVisitor_visit_alias_pinfo, _arg);
            _arg[0].u_boolean = _this->visit_alias(name);
            _b.reply(_XfStyleVisitor_visit_alias_pinfo, _arg);
            break;
        }
        case /* visit_attribute */ 1: {
            extern MarshalBuffer::ArgInfo _XfStyleVisitor_visit_attribute_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleValueRef a;
            _arg[1].u_addr = &a;
            _b.receive(_XfStyleVisitor_visit_attribute_pinfo, _arg);
            _arg[0].u_boolean = _this->visit_attribute(a);
            _b.reply(_XfStyleVisitor_visit_attribute_pinfo, _arg);
            break;
        }
        case /* visit_style */ 2: {
            extern MarshalBuffer::ArgInfo _XfStyleVisitor_visit_style_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            StyleRef s;
            _arg[1].u_addr = &s;
            _b.receive(_XfStyleVisitor_visit_style_pinfo, _arg);
            _arg[0].u_boolean = _this->visit_style(s);
            _b.reply(_XfStyleVisitor_visit_style_pinfo, _arg);
            break;
        }
    }
}
StyleVisitorStub::StyleVisitorStub(Exchange* e) { exch_ = e; }
StyleVisitorStub::~StyleVisitorStub() { }
BaseObjectRef _XfStyleVisitorStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new StyleVisitorStub(e);
}
Exchange* StyleVisitorStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfStyleVisitor_visit_alias_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfStyleVisitor_visit_alias_pfunc[] = {
    &_XfCharStringStub_create,

};
MarshalBuffer::ArgInfo _XfStyleVisitor_visit_alias_pinfo = {
    &_XfStyleVisitor_tid, 0, _XfStyleVisitor_visit_alias_pdesc, _XfStyleVisitor_visit_alias_pfunc
};
Boolean StyleVisitor::visit_alias(CharString_in name) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleVisitor_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = name;
    _b.invoke(this, _XfStyleVisitor_visit_alias_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleVisitor_visit_attribute_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfStyleVisitor_visit_attribute_pfunc[] = {
    &_XfStyleValueStub_create,

};
MarshalBuffer::ArgInfo _XfStyleVisitor_visit_attribute_pinfo = {
    &_XfStyleVisitor_tid, 1, _XfStyleVisitor_visit_attribute_pdesc, _XfStyleVisitor_visit_attribute_pfunc
};
Boolean StyleVisitor::visit_attribute(StyleValue_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleVisitor_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = a;
    _b.invoke(this, _XfStyleVisitor_visit_attribute_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfStyleVisitor_visit_style_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfStyleVisitor_visit_style_pfunc[] = {
    &_XfStyleStub_create,

};
MarshalBuffer::ArgInfo _XfStyleVisitor_visit_style_pinfo = {
    &_XfStyleVisitor_tid, 2, _XfStyleVisitor_visit_style_pdesc, _XfStyleVisitor_visit_style_pfunc
};
Boolean StyleVisitor::visit_style(Style_in s) {
    MarshalBuffer _b;
    extern TypeObjId _XfStyleVisitor_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = s;
    _b.invoke(this, _XfStyleVisitor_visit_style_pinfo, _arg);
    return _arg[0].u_boolean;
}
//+

//+ Transform::%init,type+dii,client
Transform::Transform() { }
Transform::~Transform() { }
void* Transform::_this() { return this; }

extern TypeObj_Descriptor _XfTransform_Matrix_type, _XfTransform_Matrix_type, 
    _XfFloat_type;

TypeObj_OpData _XfTransform_methods[] = {
    { "load", &_Xfvoid_type, 1 },
    { "load_identity", &_Xfvoid_type, 0 },
    { "load_matrix", &_Xfvoid_type, 1 },
    { "store_matrix", &_Xfvoid_type, 1 },
    { "equal", &_XfBoolean_type, 1 },
    { "identity", &_XfBoolean_type, 0 },
    { "det_is_zero", &_XfBoolean_type, 0 },
    { "scale", &_Xfvoid_type, 1 },
    { "rotate", &_Xfvoid_type, 2 },
    { "translate", &_Xfvoid_type, 1 },
    { "premultiply", &_Xfvoid_type, 1 },
    { "postmultiply", &_Xfvoid_type, 1 },
    { "invert", &_Xfvoid_type, 0 },
    { "transform_vertex", &_Xfvoid_type, 1 },
    { "inverse_transform_vertex", &_Xfvoid_type, 1 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfTransform_params[] = {
    /* load */
        { "t", 0, &_XfTransform_type },
    /* load_matrix */
        { "m", 0, &_XfTransform_Matrix_type },
    /* store_matrix */
        { "m", 1, &_XfTransform_Matrix_type },
    /* equal */
        { "t", 0, &_XfTransform_type },
    /* scale */
        { "v", 0, &_XfVertex_type },
    /* rotate */
        { "angle", 0, &_XfFloat_type },
        { "a", 0, &_XfAxis_type },
    /* translate */
        { "v", 0, &_XfVertex_type },
    /* premultiply */
        { "t", 0, &_XfTransform_type },
    /* postmultiply */
        { "t", 0, &_XfTransform_type },
    /* transform_vertex */
        { "v", 2, &_XfVertex_type },
    /* inverse_transform_vertex */
        { "v", 2, &_XfVertex_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfTransform_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfTransform_tid;
extern void _XfTransform_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfTransform_type = {
    /* type */ 0,
    /* id */ &_XfTransform_tid,
    "Transform",
    _XfTransform_parents, /* offsets */ nil, /* excepts */ nil,
    _XfTransform_methods, _XfTransform_params,
    &_XfTransform_receive
};

TransformRef Transform::_narrow(BaseObjectRef o) {
    return (TransformRef)_BaseObject_tnarrow(
        o, _XfTransform_tid, &_XfTransformStub_create
    );
}
TypeObjId Transform::_tid() { return _XfTransform_tid; }
void _XfTransform_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfTransform_tid;
    TransformRef _this = (TransformRef)_BaseObject_tcast(_object, _XfTransform_tid);
    switch (_m) {
        case /* load */ 0: {
            extern MarshalBuffer::ArgInfo _XfTransform_load_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TransformRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfTransform_load_pinfo, _arg);
            _this->load(t);
            _b.reply(_XfTransform_load_pinfo, _arg);
            break;
        }
        case /* load_identity */ 1: {
            extern MarshalBuffer::ArgInfo _XfTransform_load_identity_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->load_identity();
            _b.reply(_XfTransform_load_identity_pinfo, _arg);
            break;
        }
        case /* load_matrix */ 2: {
            extern MarshalBuffer::ArgInfo _XfTransform_load_matrix_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Transform::Matrix m;
            _arg[1].u_addr = m;
            _b.receive(_XfTransform_load_matrix_pinfo, _arg);
            _this->load_matrix(m);
            _b.reply(_XfTransform_load_matrix_pinfo, _arg);
            break;
        }
        case /* store_matrix */ 3: {
            extern MarshalBuffer::ArgInfo _XfTransform_store_matrix_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Transform::Matrix m;
            _arg[1].u_addr = m;
            _b.receive(_XfTransform_store_matrix_pinfo, _arg);
            _this->store_matrix(m);
            _b.reply(_XfTransform_store_matrix_pinfo, _arg);
            break;
        }
        case /* equal */ 4: {
            extern MarshalBuffer::ArgInfo _XfTransform_equal_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TransformRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfTransform_equal_pinfo, _arg);
            _arg[0].u_boolean = _this->equal(t);
            _b.reply(_XfTransform_equal_pinfo, _arg);
            break;
        }
        case /* identity */ 5: {
            extern MarshalBuffer::ArgInfo _XfTransform_identity_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->identity();
            _b.reply(_XfTransform_identity_pinfo, _arg);
            break;
        }
        case /* det_is_zero */ 6: {
            extern MarshalBuffer::ArgInfo _XfTransform_det_is_zero_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->det_is_zero();
            _b.reply(_XfTransform_det_is_zero_pinfo, _arg);
            break;
        }
        case /* scale */ 7: {
            extern MarshalBuffer::ArgInfo _XfTransform_scale_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Vertex v;
            _arg[1].u_addr = &v;
            _b.receive(_XfTransform_scale_pinfo, _arg);
            _this->scale(v);
            _b.reply(_XfTransform_scale_pinfo, _arg);
            break;
        }
        case /* rotate */ 8: {
            extern MarshalBuffer::ArgInfo _XfTransform_rotate_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Float angle;
            _arg[1].u_addr = &angle;
            Axis a;
            _arg[2].u_addr = &a;
            _b.receive(_XfTransform_rotate_pinfo, _arg);
            _this->rotate(angle, a);
            _b.reply(_XfTransform_rotate_pinfo, _arg);
            break;
        }
        case /* translate */ 9: {
            extern MarshalBuffer::ArgInfo _XfTransform_translate_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Vertex v;
            _arg[1].u_addr = &v;
            _b.receive(_XfTransform_translate_pinfo, _arg);
            _this->translate(v);
            _b.reply(_XfTransform_translate_pinfo, _arg);
            break;
        }
        case /* premultiply */ 10: {
            extern MarshalBuffer::ArgInfo _XfTransform_premultiply_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TransformRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfTransform_premultiply_pinfo, _arg);
            _this->premultiply(t);
            _b.reply(_XfTransform_premultiply_pinfo, _arg);
            break;
        }
        case /* postmultiply */ 11: {
            extern MarshalBuffer::ArgInfo _XfTransform_postmultiply_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            TransformRef t;
            _arg[1].u_addr = &t;
            _b.receive(_XfTransform_postmultiply_pinfo, _arg);
            _this->postmultiply(t);
            _b.reply(_XfTransform_postmultiply_pinfo, _arg);
            break;
        }
        case /* invert */ 12: {
            extern MarshalBuffer::ArgInfo _XfTransform_invert_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->invert();
            _b.reply(_XfTransform_invert_pinfo, _arg);
            break;
        }
        case /* transform_vertex */ 13: {
            extern MarshalBuffer::ArgInfo _XfTransform_transform_vertex_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Vertex v;
            _arg[1].u_addr = &v;
            _b.receive(_XfTransform_transform_vertex_pinfo, _arg);
            _this->transform_vertex(v);
            _b.reply(_XfTransform_transform_vertex_pinfo, _arg);
            break;
        }
        case /* inverse_transform_vertex */ 14: {
            extern MarshalBuffer::ArgInfo _XfTransform_inverse_transform_vertex_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Vertex v;
            _arg[1].u_addr = &v;
            _b.receive(_XfTransform_inverse_transform_vertex_pinfo, _arg);
            _this->inverse_transform_vertex(v);
            _b.reply(_XfTransform_inverse_transform_vertex_pinfo, _arg);
            break;
        }
    }
}
extern void _XfTransform_Matrix_put(
    MarshalBuffer&, const Coord Matrix[4][4]
);
extern void _XfTransform_Matrix_get(
    MarshalBuffer&, Coord Matrix[4][4]
);
extern void _XfTransform_Matrix_put(
    MarshalBuffer&, const Coord Matrix[4][4]
);
extern void _XfTransform_Matrix_get(
    MarshalBuffer&, Coord Matrix[4][4]
);
extern void _XfTransform_Matrix_put(
    MarshalBuffer&, const Coord Matrix[4][4]
);
extern void _XfTransform_Matrix_get(
    MarshalBuffer&, Coord Matrix[4][4]
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);
extern void _XfVertex_put(
    MarshalBuffer&, const Vertex&
);
extern void _XfVertex_get(
    MarshalBuffer&, Vertex&
);

TransformStub::TransformStub(Exchange* e) { exch_ = e; }
TransformStub::~TransformStub() { }
BaseObjectRef _XfTransformStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new TransformStub(e);
}
Exchange* TransformStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfTransform_load_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfTransform_load_pfunc[] = {
    &_XfTransformStub_create,

};
MarshalBuffer::ArgInfo _XfTransform_load_pinfo = {
    &_XfTransform_tid, 0, _XfTransform_load_pdesc, _XfTransform_load_pfunc
};
void Transform::load(Transform_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfTransform_load_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_load_identity_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfTransform_load_identity_pinfo = {
    &_XfTransform_tid, 1, _XfTransform_load_identity_pdesc, 0
};
void Transform::load_identity() {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfTransform_load_identity_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_load_matrix_pdesc[3] = { 2, 4, 1 };
MarshalBuffer::ArgMarshal _XfTransform_load_matrix_pfunc[] = {
    &_XfTransform_Matrix_put, &_XfTransform_Matrix_get,

};
MarshalBuffer::ArgInfo _XfTransform_load_matrix_pinfo = {
    &_XfTransform_tid, 2, _XfTransform_load_matrix_pdesc, _XfTransform_load_matrix_pfunc
};
void Transform::load_matrix(Matrix m) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = m;
    _b.invoke(this, _XfTransform_load_matrix_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_store_matrix_pdesc[3] = { 2, 4, 2 };
MarshalBuffer::ArgMarshal _XfTransform_store_matrix_pfunc[] = {
    &_XfTransform_Matrix_put, &_XfTransform_Matrix_get,

};
MarshalBuffer::ArgInfo _XfTransform_store_matrix_pinfo = {
    &_XfTransform_tid, 3, _XfTransform_store_matrix_pdesc, _XfTransform_store_matrix_pfunc
};
void Transform::store_matrix(Matrix m) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = m;
    _b.invoke(this, _XfTransform_store_matrix_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_equal_pdesc[3] = { 2, 12, 61 };
MarshalBuffer::ArgMarshal _XfTransform_equal_pfunc[] = {
    &_XfTransformStub_create,

};
MarshalBuffer::ArgInfo _XfTransform_equal_pinfo = {
    &_XfTransform_tid, 4, _XfTransform_equal_pdesc, _XfTransform_equal_pfunc
};
Boolean Transform::equal(Transform_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfTransform_equal_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfTransform_identity_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfTransform_identity_pinfo = {
    &_XfTransform_tid, 5, _XfTransform_identity_pdesc, 0
};
Boolean Transform::identity() {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfTransform_identity_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfTransform_det_is_zero_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfTransform_det_is_zero_pinfo = {
    &_XfTransform_tid, 6, _XfTransform_det_is_zero_pdesc, 0
};
Boolean Transform::det_is_zero() {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfTransform_det_is_zero_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfTransform_scale_pdesc[3] = { 2, 4, 1 };
MarshalBuffer::ArgMarshal _XfTransform_scale_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfTransform_scale_pinfo = {
    &_XfTransform_tid, 7, _XfTransform_scale_pdesc, _XfTransform_scale_pfunc
};
void Transform::scale(const Vertex& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &v;
    _b.invoke(this, _XfTransform_scale_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_rotate_pdesc[4] = { 3, 4, 49, 33 };
MarshalBuffer::ArgInfo _XfTransform_rotate_pinfo = {
    &_XfTransform_tid, 8, _XfTransform_rotate_pdesc, 0
};
void Transform::rotate(Float angle, Axis a) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_float = angle;
    _arg[2].u_long = a;
    _b.invoke(this, _XfTransform_rotate_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_translate_pdesc[3] = { 2, 4, 1 };
MarshalBuffer::ArgMarshal _XfTransform_translate_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfTransform_translate_pinfo = {
    &_XfTransform_tid, 9, _XfTransform_translate_pdesc, _XfTransform_translate_pfunc
};
void Transform::translate(const Vertex& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &v;
    _b.invoke(this, _XfTransform_translate_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_premultiply_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfTransform_premultiply_pfunc[] = {
    &_XfTransformStub_create,

};
MarshalBuffer::ArgInfo _XfTransform_premultiply_pinfo = {
    &_XfTransform_tid, 10, _XfTransform_premultiply_pdesc, _XfTransform_premultiply_pfunc
};
void Transform::premultiply(Transform_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfTransform_premultiply_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_postmultiply_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfTransform_postmultiply_pfunc[] = {
    &_XfTransformStub_create,

};
MarshalBuffer::ArgInfo _XfTransform_postmultiply_pinfo = {
    &_XfTransform_tid, 11, _XfTransform_postmultiply_pdesc, _XfTransform_postmultiply_pfunc
};
void Transform::postmultiply(Transform_in t) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = t;
    _b.invoke(this, _XfTransform_postmultiply_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_invert_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfTransform_invert_pinfo = {
    &_XfTransform_tid, 12, _XfTransform_invert_pdesc, 0
};
void Transform::invert() {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfTransform_invert_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_transform_vertex_pdesc[3] = { 2, 4, 3 };
MarshalBuffer::ArgMarshal _XfTransform_transform_vertex_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfTransform_transform_vertex_pinfo = {
    &_XfTransform_tid, 13, _XfTransform_transform_vertex_pdesc, _XfTransform_transform_vertex_pfunc
};
void Transform::transform_vertex(Vertex& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &v;
    _b.invoke(this, _XfTransform_transform_vertex_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfTransform_inverse_transform_vertex_pdesc[3] = { 2, 4, 3 };
MarshalBuffer::ArgMarshal _XfTransform_inverse_transform_vertex_pfunc[] = {
    &_XfVertex_put, &_XfVertex_get,

};
MarshalBuffer::ArgInfo _XfTransform_inverse_transform_vertex_pinfo = {
    &_XfTransform_tid, 14, _XfTransform_inverse_transform_vertex_pdesc, _XfTransform_inverse_transform_vertex_pfunc
};
void Transform::inverse_transform_vertex(Vertex& v) {
    MarshalBuffer _b;
    extern TypeObjId _XfTransform_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_addr = &v;
    _b.invoke(this, _XfTransform_inverse_transform_vertex_pinfo, _arg);
}
void _XfTransform_Matrix_put(MarshalBuffer& _b, const Coord _array[4][4]
) {
    for (int _i0 = 0; _i0 < 4; _i0++) {
        for (int _i1 = 0; _i1 < 4; _i1++) {
            const Coord& _tmp = _array[_i0][_i1];
            _b.put_float(_tmp);
        }
    }
}
void _XfTransform_Matrix_get(MarshalBuffer& _b, Coord _array[4][4]
) {
    for (int _i0 = 0; _i0 < 4; _i0++) {
        for (int _i1 = 0; _i1 < 4; _i1++) {
            Coord& _tmp = _array[_i0][_i1];
            _tmp = _b.get_float();
        }
    }
}
//+

//+ Vertex
void _XfVertex_put(MarshalBuffer& _b, const Vertex& _this) {
    _b.put_float(_this.x);
    _b.put_float(_this.y);
    _b.put_float(_this.z);
}
void _XfVertex_get(MarshalBuffer& _b, Vertex& _this) {
    _this.x = _b.get_float();
    _this.y = _b.get_float();
    _this.z = _b.get_float();
}
//+
