/*
 * $XConsortium: Sviewers.cxx,v 1.2 94/04/01 16:48:34 matt Exp $
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
 * Stubs for Viewer and related interfaces
 */

#include <X11/Fresco/viewer.h>
#include <X11/Fresco/Impls/Xevent.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/schema.h>

//+ Event::%init,type+dii,client
Event::KeyChord& Event::KeyChord::operator =(const KeyChord& _s) {
    delete [] _buffer;
    _maximum = _s._maximum;
    _length = _s._length;
    _buffer = _maximum == 0 ? 0 : new KeySym[_maximum];
    for (int i = 0; i < _length; i++) {
        _buffer[i] = _s._buffer[i];
    }
    return *this;
}

Event::Event() { }
Event::~Event() { }
void* Event::_this() { return this; }

extern TypeObj_Descriptor _XfEvent_TypeId_type, _XfEvent_TimeStamp_type, 
    _XfBoolean_type, _XfCoord_type, _XfEvent_ButtonIndex_type, _XfEvent_Modifier_type, 
    _XfEvent_KeySym_type, _XfCharCode_type, _Xfvoid_type;

TypeObj_OpData _XfEvent_methods[] = {
    { "type", &_XfEvent_TypeId_type, 0 },
    { "time", &_XfEvent_TimeStamp_type, 0 },
    { "double_click", &_XfBoolean_type, 1 },
    { "positional", &_XfBoolean_type, 0 },
    { "pointer_x", &_XfCoord_type, 0 },
    { "pointer_y", &_XfCoord_type, 0 },
    { "pointer_button", &_XfEvent_ButtonIndex_type, 0 },
    { "button_is_down", &_XfBoolean_type, 1 },
    { "modifier_is_down", &_XfBoolean_type, 1 },
    { "key", &_XfEvent_KeySym_type, 0 },
    { "character", &_XfCharCode_type, 0 },
    { "unread", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfEvent_params[] = {
    /* double_click */
        { "previous", 0, &_XfEvent_TimeStamp_type },
    /* button_is_down */
        { "b", 0, &_XfEvent_ButtonIndex_type },
    /* modifier_is_down */
        { "m", 0, &_XfEvent_Modifier_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfEvent_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfEvent_tid;
extern void _XfEvent_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfEvent_type = {
    /* type */ 0,
    /* id */ &_XfEvent_tid,
    "Event",
    _XfEvent_parents, /* offsets */ nil, /* excepts */ nil,
    _XfEvent_methods, _XfEvent_params,
    &_XfEvent_receive
};

EventRef Event::_narrow(BaseObjectRef o) {
    return (EventRef)_BaseObject_tnarrow(
        o, _XfEvent_tid, &_XfEventStub_create
    );
}
TypeObjId Event::_tid() { return _XfEvent_tid; }
void _XfEvent_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfEvent_tid;
    EventRef _this = (EventRef)_BaseObject_tcast(_object, _XfEvent_tid);
    switch (_m) {
        case /* type */ 0: {
            extern MarshalBuffer::ArgInfo _XfEvent_type_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->type();
            _b.reply(_XfEvent_type_pinfo, _arg);
            break;
        }
        case /* time */ 1: {
            extern MarshalBuffer::ArgInfo _XfEvent_time_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_unsigned_long = _this->time();
            _b.reply(_XfEvent_time_pinfo, _arg);
            break;
        }
        case /* double_click */ 2: {
            extern MarshalBuffer::ArgInfo _XfEvent_double_click_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Event::TimeStamp previous;
            _arg[1].u_addr = &previous;
            _b.receive(_XfEvent_double_click_pinfo, _arg);
            _arg[0].u_boolean = _this->double_click(previous);
            _b.reply(_XfEvent_double_click_pinfo, _arg);
            break;
        }
        case /* positional */ 3: {
            extern MarshalBuffer::ArgInfo _XfEvent_positional_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->positional();
            _b.reply(_XfEvent_positional_pinfo, _arg);
            break;
        }
        case /* pointer_x */ 4: {
            extern MarshalBuffer::ArgInfo _XfEvent_pointer_x_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->pointer_x();
            _b.reply(_XfEvent_pointer_x_pinfo, _arg);
            break;
        }
        case /* pointer_y */ 5: {
            extern MarshalBuffer::ArgInfo _XfEvent_pointer_y_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_float = _this->pointer_y();
            _b.reply(_XfEvent_pointer_y_pinfo, _arg);
            break;
        }
        case /* pointer_button */ 6: {
            extern MarshalBuffer::ArgInfo _XfEvent_pointer_button_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->pointer_button();
            _b.reply(_XfEvent_pointer_button_pinfo, _arg);
            break;
        }
        case /* button_is_down */ 7: {
            extern MarshalBuffer::ArgInfo _XfEvent_button_is_down_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Event::ButtonIndex b;
            _arg[1].u_addr = &b;
            _b.receive(_XfEvent_button_is_down_pinfo, _arg);
            _arg[0].u_boolean = _this->button_is_down(b);
            _b.reply(_XfEvent_button_is_down_pinfo, _arg);
            break;
        }
        case /* modifier_is_down */ 8: {
            extern MarshalBuffer::ArgInfo _XfEvent_modifier_is_down_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Event::Modifier m;
            _arg[1].u_addr = &m;
            _b.receive(_XfEvent_modifier_is_down_pinfo, _arg);
            _arg[0].u_boolean = _this->modifier_is_down(m);
            _b.reply(_XfEvent_modifier_is_down_pinfo, _arg);
            break;
        }
        case /* key */ 9: {
            extern MarshalBuffer::ArgInfo _XfEvent_key_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_unsigned_long = _this->key();
            _b.reply(_XfEvent_key_pinfo, _arg);
            break;
        }
        case /* character */ 10: {
            extern MarshalBuffer::ArgInfo _XfEvent_character_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_long = _this->character();
            _b.reply(_XfEvent_character_pinfo, _arg);
            break;
        }
        case /* unread */ 11: {
            extern MarshalBuffer::ArgInfo _XfEvent_unread_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->unread();
            _b.reply(_XfEvent_unread_pinfo, _arg);
            break;
        }
    }
}
extern void _XfEvent_KeyChord_put(
    MarshalBuffer&, const Event::KeyChord&
);
extern void _XfEvent_KeyChord_get(
    MarshalBuffer&, Event::KeyChord&
);

EventStub::EventStub(Exchange* e) { exch_ = e; }
EventStub::~EventStub() { }
BaseObjectRef _XfEventStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new EventStub(e);
}
Exchange* EventStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfEvent_type_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfEvent_type_pinfo = {
    &_XfEvent_tid, 0, _XfEvent_type_pdesc, 0
};
Event::TypeId Event::type() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_type_pinfo, _arg);
    return (Event::TypeId)_arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfEvent_time_pdesc[2] = { 1, 36 };
MarshalBuffer::ArgInfo _XfEvent_time_pinfo = {
    &_XfEvent_tid, 1, _XfEvent_time_pdesc, 0
};
Event::TimeStamp Event::time() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_time_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfEvent_double_click_pdesc[3] = { 2, 12, 37 };
MarshalBuffer::ArgInfo _XfEvent_double_click_pinfo = {
    &_XfEvent_tid, 2, _XfEvent_double_click_pdesc, 0
};
Boolean Event::double_click(Event::TimeStamp previous) {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_unsigned_long = previous;
    _b.invoke(this, _XfEvent_double_click_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfEvent_positional_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfEvent_positional_pinfo = {
    &_XfEvent_tid, 3, _XfEvent_positional_pdesc, 0
};
Boolean Event::positional() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_positional_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfEvent_pointer_x_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfEvent_pointer_x_pinfo = {
    &_XfEvent_tid, 4, _XfEvent_pointer_x_pdesc, 0
};
Coord Event::pointer_x() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_pointer_x_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfEvent_pointer_y_pdesc[2] = { 1, 48 };
MarshalBuffer::ArgInfo _XfEvent_pointer_y_pinfo = {
    &_XfEvent_tid, 5, _XfEvent_pointer_y_pdesc, 0
};
Coord Event::pointer_y() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_pointer_y_pinfo, _arg);
    return _arg[0].u_float;
}
MarshalBuffer::ArgDesc _XfEvent_pointer_button_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfEvent_pointer_button_pinfo = {
    &_XfEvent_tid, 6, _XfEvent_pointer_button_pdesc, 0
};
Event::ButtonIndex Event::pointer_button() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_pointer_button_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfEvent_button_is_down_pdesc[3] = { 2, 12, 33 };
MarshalBuffer::ArgInfo _XfEvent_button_is_down_pinfo = {
    &_XfEvent_tid, 7, _XfEvent_button_is_down_pdesc, 0
};
Boolean Event::button_is_down(Event::ButtonIndex b) {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = b;
    _b.invoke(this, _XfEvent_button_is_down_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfEvent_modifier_is_down_pdesc[3] = { 2, 12, 33 };
MarshalBuffer::ArgInfo _XfEvent_modifier_is_down_pinfo = {
    &_XfEvent_tid, 8, _XfEvent_modifier_is_down_pdesc, 0
};
Boolean Event::modifier_is_down(Event::Modifier m) {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_long = m;
    _b.invoke(this, _XfEvent_modifier_is_down_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfEvent_key_pdesc[2] = { 1, 36 };
MarshalBuffer::ArgInfo _XfEvent_key_pinfo = {
    &_XfEvent_tid, 9, _XfEvent_key_pdesc, 0
};
Event::KeySym Event::key() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_key_pinfo, _arg);
    return _arg[0].u_unsigned_long;
}
MarshalBuffer::ArgDesc _XfEvent_character_pdesc[2] = { 1, 32 };
MarshalBuffer::ArgInfo _XfEvent_character_pinfo = {
    &_XfEvent_tid, 10, _XfEvent_character_pdesc, 0
};
CharCode Event::character() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_character_pinfo, _arg);
    return _arg[0].u_long;
}
MarshalBuffer::ArgDesc _XfEvent_unread_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfEvent_unread_pinfo = {
    &_XfEvent_tid, 11, _XfEvent_unread_pdesc, 0
};
void Event::unread() {
    MarshalBuffer _b;
    extern TypeObjId _XfEvent_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfEvent_unread_pinfo, _arg);
}
void _XfEvent_KeyChord_put(MarshalBuffer& _b, const Event::KeyChord& _this) {
    _b.put_seq(&_this, sizeof(Event::KeySym));
}
void _XfEvent_KeyChord_get(MarshalBuffer& _b, Event::KeyChord& _this) {
    _b.get_seq(&_this, sizeof(Event::KeySym));
}
//+

//+ EventImpl(Event)
extern TypeObj_Descriptor _XfEvent_type;
TypeObj_Descriptor* _XfEventImpl_parents[] = { &_XfEvent_type, nil };
extern TypeObjId _XfEventImpl_tid;
TypeObj_Descriptor _XfEventImpl_type = {
    /* type */ 0,
    /* id */ &_XfEventImpl_tid,
    "EventImpl",
    _XfEventImpl_parents, /* offsets */ nil, /* excepts */ nil,
    /* methods */ nil, /* params */ nil,
    /* receive */ nil
};

EventImpl* EventImpl::_narrow(BaseObjectRef o) {
    return (EventImpl*)_BaseObject_tnarrow(
        o, _XfEventImpl_tid, 0
    );
}
TypeObjId EventImpl::_tid() { return _XfEventImpl_tid; }
//+

//+ Focus::%init,type+dii,client
Focus::Focus() { }
Focus::~Focus() { }
void* Focus::_this() { return this; }

extern TypeObj_Descriptor _XfViewer_type, _XfAction_type, _XfEvent_KeyChord_type;

TypeObj_OpData _XfFocus_methods[] = {
    { "add_focus_interest", &_Xfvoid_type, 1 },
    { "receive_focus_below", &_Xfvoid_type, 2 },
    { "lose_focus_below", &_Xfvoid_type, 2 },
    { "map_keystroke", &_Xfvoid_type, 2 },
    { "map_keychord", &_Xfvoid_type, 2 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfFocus_params[] = {
    /* add_focus_interest */
        { "v", 0, &_XfViewer_type },
    /* receive_focus_below */
        { "v", 0, &_XfViewer_type },
        { "temporary", 0, &_XfBoolean_type },
    /* lose_focus_below */
        { "v", 0, &_XfViewer_type },
        { "temporary", 0, &_XfBoolean_type },
    /* map_keystroke */
        { "k", 0, &_XfEvent_KeySym_type },
        { "a", 0, &_XfAction_type },
    /* map_keychord */
        { "k", 0, &_XfEvent_KeyChord_type },
        { "a", 0, &_XfAction_type }
};
extern TypeObj_Descriptor _XfFrescoObject_type;
TypeObj_Descriptor* _XfFocus_parents[] = { &_XfFrescoObject_type, nil };
extern TypeObjId _XfFocus_tid;
extern void _XfFocus_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfFocus_type = {
    /* type */ 0,
    /* id */ &_XfFocus_tid,
    "Focus",
    _XfFocus_parents, /* offsets */ nil, /* excepts */ nil,
    _XfFocus_methods, _XfFocus_params,
    &_XfFocus_receive
};

FocusRef Focus::_narrow(BaseObjectRef o) {
    return (FocusRef)_BaseObject_tnarrow(
        o, _XfFocus_tid, &_XfFocusStub_create
    );
}
TypeObjId Focus::_tid() { return _XfFocus_tid; }
void _XfFocus_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfFocus_tid;
    FocusRef _this = (FocusRef)_BaseObject_tcast(_object, _XfFocus_tid);
    switch (_m) {
        case /* add_focus_interest */ 0: {
            extern MarshalBuffer::ArgInfo _XfFocus_add_focus_interest_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfFocus_add_focus_interest_pinfo, _arg);
            _this->add_focus_interest(v);
            _b.reply(_XfFocus_add_focus_interest_pinfo, _arg);
            break;
        }
        case /* receive_focus_below */ 1: {
            extern MarshalBuffer::ArgInfo _XfFocus_receive_focus_below_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            ViewerRef v;
            _arg[1].u_addr = &v;
            Boolean temporary;
            _arg[2].u_addr = &temporary;
            _b.receive(_XfFocus_receive_focus_below_pinfo, _arg);
            _this->receive_focus_below(v, temporary);
            _b.reply(_XfFocus_receive_focus_below_pinfo, _arg);
            break;
        }
        case /* lose_focus_below */ 2: {
            extern MarshalBuffer::ArgInfo _XfFocus_lose_focus_below_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            ViewerRef v;
            _arg[1].u_addr = &v;
            Boolean temporary;
            _arg[2].u_addr = &temporary;
            _b.receive(_XfFocus_lose_focus_below_pinfo, _arg);
            _this->lose_focus_below(v, temporary);
            _b.reply(_XfFocus_lose_focus_below_pinfo, _arg);
            break;
        }
        case /* map_keystroke */ 3: {
            extern MarshalBuffer::ArgInfo _XfFocus_map_keystroke_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Event::KeySym k;
            _arg[1].u_addr = &k;
            ActionRef a;
            _arg[2].u_addr = &a;
            _b.receive(_XfFocus_map_keystroke_pinfo, _arg);
            _this->map_keystroke(k, a);
            _b.reply(_XfFocus_map_keystroke_pinfo, _arg);
            break;
        }
        case /* map_keychord */ 4: {
            extern MarshalBuffer::ArgInfo _XfFocus_map_keychord_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            Event::KeyChord k;
            _arg[1].u_addr = &k;
            ActionRef a;
            _arg[2].u_addr = &a;
            _b.receive(_XfFocus_map_keychord_pinfo, _arg);
            _this->map_keychord(k, a);
            _b.reply(_XfFocus_map_keychord_pinfo, _arg);
            break;
        }
    }
}
extern void _XfEvent_KeyChord_put(
    MarshalBuffer&, const Event::KeyChord&
);
extern void _XfEvent_KeyChord_get(
    MarshalBuffer&, Event::KeyChord&
);

FocusStub::FocusStub(Exchange* e) { exch_ = e; }
FocusStub::~FocusStub() { }
BaseObjectRef _XfFocusStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new FocusStub(e);
}
Exchange* FocusStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfFocus_add_focus_interest_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfFocus_add_focus_interest_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfFocus_add_focus_interest_pinfo = {
    &_XfFocus_tid, 0, _XfFocus_add_focus_interest_pdesc, _XfFocus_add_focus_interest_pfunc
};
void Focus::add_focus_interest(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfFocus_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfFocus_add_focus_interest_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFocus_receive_focus_below_pdesc[4] = { 3, 4, 61, 13 };
MarshalBuffer::ArgMarshal _XfFocus_receive_focus_below_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfFocus_receive_focus_below_pinfo = {
    &_XfFocus_tid, 1, _XfFocus_receive_focus_below_pdesc, _XfFocus_receive_focus_below_pfunc
};
void Focus::receive_focus_below(Viewer_in v, Boolean temporary) {
    MarshalBuffer _b;
    extern TypeObjId _XfFocus_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = v;
    _arg[2].u_boolean = temporary;
    _b.invoke(this, _XfFocus_receive_focus_below_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFocus_lose_focus_below_pdesc[4] = { 3, 4, 61, 13 };
MarshalBuffer::ArgMarshal _XfFocus_lose_focus_below_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfFocus_lose_focus_below_pinfo = {
    &_XfFocus_tid, 2, _XfFocus_lose_focus_below_pdesc, _XfFocus_lose_focus_below_pfunc
};
void Focus::lose_focus_below(Viewer_in v, Boolean temporary) {
    MarshalBuffer _b;
    extern TypeObjId _XfFocus_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = v;
    _arg[2].u_boolean = temporary;
    _b.invoke(this, _XfFocus_lose_focus_below_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFocus_map_keystroke_pdesc[4] = { 3, 4, 37, 61 };
MarshalBuffer::ArgMarshal _XfFocus_map_keystroke_pfunc[] = {
    &_XfActionStub_create,

};
MarshalBuffer::ArgInfo _XfFocus_map_keystroke_pinfo = {
    &_XfFocus_tid, 3, _XfFocus_map_keystroke_pdesc, _XfFocus_map_keystroke_pfunc
};
void Focus::map_keystroke(Event::KeySym k, Action_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfFocus_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_unsigned_long = k;
    _arg[2].u_objref = a;
    _b.invoke(this, _XfFocus_map_keystroke_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfFocus_map_keychord_pdesc[4] = { 3, 4, 1, 61 };
MarshalBuffer::ArgMarshal _XfFocus_map_keychord_pfunc[] = {
    &_XfEvent_KeyChord_put, &_XfEvent_KeyChord_get,
    &_XfActionStub_create,

};
MarshalBuffer::ArgInfo _XfFocus_map_keychord_pinfo = {
    &_XfFocus_tid, 4, _XfFocus_map_keychord_pdesc, _XfFocus_map_keychord_pfunc
};
void Focus::map_keychord(const Event::KeyChord& k, Action_in a) {
    MarshalBuffer _b;
    extern TypeObjId _XfFocus_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_addr = &k;
    _arg[2].u_objref = a;
    _b.invoke(this, _XfFocus_map_keychord_pinfo, _arg);
}
//+

//+ Viewer::%init,type+dii,client
Viewer::Viewer() { }
Viewer::~Viewer() { }
void* Viewer::_this() { return this; }

extern TypeObj_Descriptor _XfFocus_type, _XfGlyphTraversal_type, _XfEvent_type;

TypeObj_OpData _XfViewer_methods[] = {
    { "parent_viewer", &_XfViewer_type, 0 },
    { "next_viewer", &_XfViewer_type, 0 },
    { "prev_viewer", &_XfViewer_type, 0 },
    { "first_viewer", &_XfViewer_type, 0 },
    { "last_viewer", &_XfViewer_type, 0 },
    { "append_viewer", &_Xfvoid_type, 1 },
    { "prepend_viewer", &_Xfvoid_type, 1 },
    { "insert_viewer", &_Xfvoid_type, 1 },
    { "replace_viewer", &_Xfvoid_type, 1 },
    { "remove_viewer", &_Xfvoid_type, 0 },
    { "set_viewer_links", &_Xfvoid_type, 3 },
    { "set_first_viewer", &_Xfvoid_type, 1 },
    { "set_last_viewer", &_Xfvoid_type, 1 },
    { "request_focus", &_XfFocus_type, 2 },
    { "receive_focus", &_XfBoolean_type, 2 },
    { "lose_focus", &_Xfvoid_type, 1 },
    { "first_focus", &_XfBoolean_type, 0 },
    { "last_focus", &_XfBoolean_type, 0 },
    { "next_focus", &_XfBoolean_type, 0 },
    { "prev_focus", &_XfBoolean_type, 0 },
    { "handle", &_XfBoolean_type, 2 },
    { "close", &_Xfvoid_type, 0 },
    { 0, 0, 0 }
};
TypeObj_ParamData _XfViewer_params[] = {
    /* append_viewer */
        { "v", 0, &_XfViewer_type },
    /* prepend_viewer */
        { "v", 0, &_XfViewer_type },
    /* insert_viewer */
        { "v", 0, &_XfViewer_type },
    /* replace_viewer */
        { "v", 0, &_XfViewer_type },
    /* set_viewer_links */
        { "parent", 0, &_XfViewer_type },
        { "prev", 0, &_XfViewer_type },
        { "next", 0, &_XfViewer_type },
    /* set_first_viewer */
        { "v", 0, &_XfViewer_type },
    /* set_last_viewer */
        { "v", 0, &_XfViewer_type },
    /* request_focus */
        { "requestor", 0, &_XfViewer_type },
        { "temporary", 0, &_XfBoolean_type },
    /* receive_focus */
        { "f", 0, &_XfFocus_type },
        { "primary", 0, &_XfBoolean_type },
    /* lose_focus */
        { "temporary", 0, &_XfBoolean_type },
    /* handle */
        { "t", 0, &_XfGlyphTraversal_type },
        { "e", 0, &_XfEvent_type }
};
extern TypeObj_Descriptor _XfGlyph_type;
TypeObj_Descriptor* _XfViewer_parents[] = { &_XfGlyph_type, nil };
extern TypeObjId _XfViewer_tid;
extern void _XfViewer_receive(BaseObjectRef, ULong, MarshalBuffer&);
TypeObj_Descriptor _XfViewer_type = {
    /* type */ 0,
    /* id */ &_XfViewer_tid,
    "Viewer",
    _XfViewer_parents, /* offsets */ nil, /* excepts */ nil,
    _XfViewer_methods, _XfViewer_params,
    &_XfViewer_receive
};

ViewerRef Viewer::_narrow(BaseObjectRef o) {
    return (ViewerRef)_BaseObject_tnarrow(
        o, _XfViewer_tid, &_XfViewerStub_create
    );
}
TypeObjId Viewer::_tid() { return _XfViewer_tid; }
void _XfViewer_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _XfViewer_tid;
    ViewerRef _this = (ViewerRef)_BaseObject_tcast(_object, _XfViewer_tid);
    switch (_m) {
        case /* parent_viewer */ 0: {
            extern MarshalBuffer::ArgInfo _XfViewer_parent_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->parent_viewer();
            _b.reply(_XfViewer_parent_viewer_pinfo, _arg);
            break;
        }
        case /* next_viewer */ 1: {
            extern MarshalBuffer::ArgInfo _XfViewer_next_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->next_viewer();
            _b.reply(_XfViewer_next_viewer_pinfo, _arg);
            break;
        }
        case /* prev_viewer */ 2: {
            extern MarshalBuffer::ArgInfo _XfViewer_prev_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->prev_viewer();
            _b.reply(_XfViewer_prev_viewer_pinfo, _arg);
            break;
        }
        case /* first_viewer */ 3: {
            extern MarshalBuffer::ArgInfo _XfViewer_first_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->first_viewer();
            _b.reply(_XfViewer_first_viewer_pinfo, _arg);
            break;
        }
        case /* last_viewer */ 4: {
            extern MarshalBuffer::ArgInfo _XfViewer_last_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_objref = _this->last_viewer();
            _b.reply(_XfViewer_last_viewer_pinfo, _arg);
            break;
        }
        case /* append_viewer */ 5: {
            extern MarshalBuffer::ArgInfo _XfViewer_append_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfViewer_append_viewer_pinfo, _arg);
            _this->append_viewer(v);
            _b.reply(_XfViewer_append_viewer_pinfo, _arg);
            break;
        }
        case /* prepend_viewer */ 6: {
            extern MarshalBuffer::ArgInfo _XfViewer_prepend_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfViewer_prepend_viewer_pinfo, _arg);
            _this->prepend_viewer(v);
            _b.reply(_XfViewer_prepend_viewer_pinfo, _arg);
            break;
        }
        case /* insert_viewer */ 7: {
            extern MarshalBuffer::ArgInfo _XfViewer_insert_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfViewer_insert_viewer_pinfo, _arg);
            _this->insert_viewer(v);
            _b.reply(_XfViewer_insert_viewer_pinfo, _arg);
            break;
        }
        case /* replace_viewer */ 8: {
            extern MarshalBuffer::ArgInfo _XfViewer_replace_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfViewer_replace_viewer_pinfo, _arg);
            _this->replace_viewer(v);
            _b.reply(_XfViewer_replace_viewer_pinfo, _arg);
            break;
        }
        case /* remove_viewer */ 9: {
            extern MarshalBuffer::ArgInfo _XfViewer_remove_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->remove_viewer();
            _b.reply(_XfViewer_remove_viewer_pinfo, _arg);
            break;
        }
        case /* set_viewer_links */ 10: {
            extern MarshalBuffer::ArgInfo _XfViewer_set_viewer_links_pinfo;
            MarshalBuffer::ArgValue _arg[4];
            ViewerRef parent;
            _arg[1].u_addr = &parent;
            ViewerRef prev;
            _arg[2].u_addr = &prev;
            ViewerRef next;
            _arg[3].u_addr = &next;
            _b.receive(_XfViewer_set_viewer_links_pinfo, _arg);
            _this->set_viewer_links(parent, prev, next);
            _b.reply(_XfViewer_set_viewer_links_pinfo, _arg);
            break;
        }
        case /* set_first_viewer */ 11: {
            extern MarshalBuffer::ArgInfo _XfViewer_set_first_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfViewer_set_first_viewer_pinfo, _arg);
            _this->set_first_viewer(v);
            _b.reply(_XfViewer_set_first_viewer_pinfo, _arg);
            break;
        }
        case /* set_last_viewer */ 12: {
            extern MarshalBuffer::ArgInfo _XfViewer_set_last_viewer_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            ViewerRef v;
            _arg[1].u_addr = &v;
            _b.receive(_XfViewer_set_last_viewer_pinfo, _arg);
            _this->set_last_viewer(v);
            _b.reply(_XfViewer_set_last_viewer_pinfo, _arg);
            break;
        }
        case /* request_focus */ 13: {
            extern MarshalBuffer::ArgInfo _XfViewer_request_focus_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            ViewerRef requestor;
            _arg[1].u_addr = &requestor;
            Boolean temporary;
            _arg[2].u_addr = &temporary;
            _b.receive(_XfViewer_request_focus_pinfo, _arg);
            _arg[0].u_objref = _this->request_focus(requestor, temporary);
            _b.reply(_XfViewer_request_focus_pinfo, _arg);
            break;
        }
        case /* receive_focus */ 14: {
            extern MarshalBuffer::ArgInfo _XfViewer_receive_focus_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            FocusRef f;
            _arg[1].u_addr = &f;
            Boolean primary;
            _arg[2].u_addr = &primary;
            _b.receive(_XfViewer_receive_focus_pinfo, _arg);
            _arg[0].u_boolean = _this->receive_focus(f, primary);
            _b.reply(_XfViewer_receive_focus_pinfo, _arg);
            break;
        }
        case /* lose_focus */ 15: {
            extern MarshalBuffer::ArgInfo _XfViewer_lose_focus_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            Boolean temporary;
            _arg[1].u_addr = &temporary;
            _b.receive(_XfViewer_lose_focus_pinfo, _arg);
            _this->lose_focus(temporary);
            _b.reply(_XfViewer_lose_focus_pinfo, _arg);
            break;
        }
        case /* first_focus */ 16: {
            extern MarshalBuffer::ArgInfo _XfViewer_first_focus_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->first_focus();
            _b.reply(_XfViewer_first_focus_pinfo, _arg);
            break;
        }
        case /* last_focus */ 17: {
            extern MarshalBuffer::ArgInfo _XfViewer_last_focus_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->last_focus();
            _b.reply(_XfViewer_last_focus_pinfo, _arg);
            break;
        }
        case /* next_focus */ 18: {
            extern MarshalBuffer::ArgInfo _XfViewer_next_focus_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->next_focus();
            _b.reply(_XfViewer_next_focus_pinfo, _arg);
            break;
        }
        case /* prev_focus */ 19: {
            extern MarshalBuffer::ArgInfo _XfViewer_prev_focus_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _arg[0].u_boolean = _this->prev_focus();
            _b.reply(_XfViewer_prev_focus_pinfo, _arg);
            break;
        }
        case /* handle */ 20: {
            extern MarshalBuffer::ArgInfo _XfViewer_handle_pinfo;
            MarshalBuffer::ArgValue _arg[3];
            GlyphTraversalRef t;
            _arg[1].u_addr = &t;
            EventRef e;
            _arg[2].u_addr = &e;
            _b.receive(_XfViewer_handle_pinfo, _arg);
            _arg[0].u_boolean = _this->handle(t, e);
            _b.reply(_XfViewer_handle_pinfo, _arg);
            break;
        }
        case /* close */ 21: {
            extern MarshalBuffer::ArgInfo _XfViewer_close_pinfo;
            MarshalBuffer::ArgValue _arg[1];
            _this->close();
            _b.reply(_XfViewer_close_pinfo, _arg);
            break;
        }
    }
}
ViewerStub::ViewerStub(Exchange* e) { exch_ = e; }
ViewerStub::~ViewerStub() { }
BaseObjectRef _XfViewerStub_create(Exchange* e) {
    return (BaseObjectRef)(void*)new ViewerStub(e);
}
Exchange* ViewerStub::_exchange() {
    return exch_;
}
MarshalBuffer::ArgDesc _XfViewer_parent_viewer_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfViewer_parent_viewer_pfunc[] = {
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfViewer_parent_viewer_pinfo = {
    &_XfViewer_tid, 0, _XfViewer_parent_viewer_pdesc, _XfViewer_parent_viewer_pfunc
};
ViewerRef Viewer::parent_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_parent_viewer_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfViewer_next_viewer_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfViewer_next_viewer_pfunc[] = {
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfViewer_next_viewer_pinfo = {
    &_XfViewer_tid, 1, _XfViewer_next_viewer_pdesc, _XfViewer_next_viewer_pfunc
};
ViewerRef Viewer::next_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_next_viewer_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfViewer_prev_viewer_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfViewer_prev_viewer_pfunc[] = {
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfViewer_prev_viewer_pinfo = {
    &_XfViewer_tid, 2, _XfViewer_prev_viewer_pdesc, _XfViewer_prev_viewer_pfunc
};
ViewerRef Viewer::prev_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_prev_viewer_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfViewer_first_viewer_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfViewer_first_viewer_pfunc[] = {
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfViewer_first_viewer_pinfo = {
    &_XfViewer_tid, 3, _XfViewer_first_viewer_pdesc, _XfViewer_first_viewer_pfunc
};
ViewerRef Viewer::first_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_first_viewer_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfViewer_last_viewer_pdesc[2] = { 1, 60 };
MarshalBuffer::ArgMarshal _XfViewer_last_viewer_pfunc[] = {
    &_XfViewerStub_create
};
MarshalBuffer::ArgInfo _XfViewer_last_viewer_pinfo = {
    &_XfViewer_tid, 4, _XfViewer_last_viewer_pdesc, _XfViewer_last_viewer_pfunc
};
ViewerRef Viewer::last_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_last_viewer_pinfo, _arg);
    return (ViewerRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfViewer_append_viewer_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfViewer_append_viewer_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_append_viewer_pinfo = {
    &_XfViewer_tid, 5, _XfViewer_append_viewer_pdesc, _XfViewer_append_viewer_pfunc
};
void Viewer::append_viewer(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfViewer_append_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_prepend_viewer_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfViewer_prepend_viewer_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_prepend_viewer_pinfo = {
    &_XfViewer_tid, 6, _XfViewer_prepend_viewer_pdesc, _XfViewer_prepend_viewer_pfunc
};
void Viewer::prepend_viewer(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfViewer_prepend_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_insert_viewer_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfViewer_insert_viewer_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_insert_viewer_pinfo = {
    &_XfViewer_tid, 7, _XfViewer_insert_viewer_pdesc, _XfViewer_insert_viewer_pfunc
};
void Viewer::insert_viewer(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfViewer_insert_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_replace_viewer_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfViewer_replace_viewer_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_replace_viewer_pinfo = {
    &_XfViewer_tid, 8, _XfViewer_replace_viewer_pdesc, _XfViewer_replace_viewer_pfunc
};
void Viewer::replace_viewer(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfViewer_replace_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_remove_viewer_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfViewer_remove_viewer_pinfo = {
    &_XfViewer_tid, 9, _XfViewer_remove_viewer_pdesc, 0
};
void Viewer::remove_viewer() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_remove_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_set_viewer_links_pdesc[5] = { 4, 4, 61, 61, 61 };
MarshalBuffer::ArgMarshal _XfViewer_set_viewer_links_pfunc[] = {
    &_XfViewerStub_create,
    &_XfViewerStub_create,
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_set_viewer_links_pinfo = {
    &_XfViewer_tid, 10, _XfViewer_set_viewer_links_pdesc, _XfViewer_set_viewer_links_pfunc
};
void Viewer::set_viewer_links(Viewer_in parent, Viewer_in prev, Viewer_in next) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[4];
    _arg[1].u_objref = parent;
    _arg[2].u_objref = prev;
    _arg[3].u_objref = next;
    _b.invoke(this, _XfViewer_set_viewer_links_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_set_first_viewer_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfViewer_set_first_viewer_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_set_first_viewer_pinfo = {
    &_XfViewer_tid, 11, _XfViewer_set_first_viewer_pdesc, _XfViewer_set_first_viewer_pfunc
};
void Viewer::set_first_viewer(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfViewer_set_first_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_set_last_viewer_pdesc[3] = { 2, 4, 61 };
MarshalBuffer::ArgMarshal _XfViewer_set_last_viewer_pfunc[] = {
    &_XfViewerStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_set_last_viewer_pinfo = {
    &_XfViewer_tid, 12, _XfViewer_set_last_viewer_pdesc, _XfViewer_set_last_viewer_pfunc
};
void Viewer::set_last_viewer(Viewer_in v) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_objref = v;
    _b.invoke(this, _XfViewer_set_last_viewer_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_request_focus_pdesc[4] = { 3, 60, 61, 13 };
MarshalBuffer::ArgMarshal _XfViewer_request_focus_pfunc[] = {
    &_XfViewerStub_create,
    &_XfFocusStub_create
};
MarshalBuffer::ArgInfo _XfViewer_request_focus_pinfo = {
    &_XfViewer_tid, 13, _XfViewer_request_focus_pdesc, _XfViewer_request_focus_pfunc
};
FocusRef Viewer::request_focus(Viewer_in requestor, Boolean temporary) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = requestor;
    _arg[2].u_boolean = temporary;
    _b.invoke(this, _XfViewer_request_focus_pinfo, _arg);
    return (FocusRef)_arg[0].u_objref;
}
MarshalBuffer::ArgDesc _XfViewer_receive_focus_pdesc[4] = { 3, 12, 61, 13 };
MarshalBuffer::ArgMarshal _XfViewer_receive_focus_pfunc[] = {
    &_XfFocusStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_receive_focus_pinfo = {
    &_XfViewer_tid, 14, _XfViewer_receive_focus_pdesc, _XfViewer_receive_focus_pfunc
};
Boolean Viewer::receive_focus(Focus_in f, Boolean primary) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = f;
    _arg[2].u_boolean = primary;
    _b.invoke(this, _XfViewer_receive_focus_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfViewer_lose_focus_pdesc[3] = { 2, 4, 13 };
MarshalBuffer::ArgInfo _XfViewer_lose_focus_pinfo = {
    &_XfViewer_tid, 15, _XfViewer_lose_focus_pdesc, 0
};
void Viewer::lose_focus(Boolean temporary) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[2];
    _arg[1].u_boolean = temporary;
    _b.invoke(this, _XfViewer_lose_focus_pinfo, _arg);
}
MarshalBuffer::ArgDesc _XfViewer_first_focus_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfViewer_first_focus_pinfo = {
    &_XfViewer_tid, 16, _XfViewer_first_focus_pdesc, 0
};
Boolean Viewer::first_focus() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_first_focus_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfViewer_last_focus_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfViewer_last_focus_pinfo = {
    &_XfViewer_tid, 17, _XfViewer_last_focus_pdesc, 0
};
Boolean Viewer::last_focus() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_last_focus_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfViewer_next_focus_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfViewer_next_focus_pinfo = {
    &_XfViewer_tid, 18, _XfViewer_next_focus_pdesc, 0
};
Boolean Viewer::next_focus() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_next_focus_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfViewer_prev_focus_pdesc[2] = { 1, 12 };
MarshalBuffer::ArgInfo _XfViewer_prev_focus_pinfo = {
    &_XfViewer_tid, 19, _XfViewer_prev_focus_pdesc, 0
};
Boolean Viewer::prev_focus() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_prev_focus_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfViewer_handle_pdesc[4] = { 3, 12, 61, 61 };
MarshalBuffer::ArgMarshal _XfViewer_handle_pfunc[] = {
    &_XfGlyphTraversalStub_create,
    &_XfEventStub_create,

};
MarshalBuffer::ArgInfo _XfViewer_handle_pinfo = {
    &_XfViewer_tid, 20, _XfViewer_handle_pdesc, _XfViewer_handle_pfunc
};
Boolean Viewer::handle(GlyphTraversal_in t, Event_in e) {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[3];
    _arg[1].u_objref = t;
    _arg[2].u_objref = e;
    _b.invoke(this, _XfViewer_handle_pinfo, _arg);
    return _arg[0].u_boolean;
}
MarshalBuffer::ArgDesc _XfViewer_close_pdesc[2] = { 1, 4 };
MarshalBuffer::ArgInfo _XfViewer_close_pinfo = {
    &_XfViewer_tid, 21, _XfViewer_close_pdesc, 0
};
void Viewer::close() {
    MarshalBuffer _b;
    extern TypeObjId _XfViewer_tid;
    MarshalBuffer::ArgValue _arg[1];
    _b.invoke(this, _XfViewer_close_pinfo, _arg);
}
//+
